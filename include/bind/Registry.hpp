#pragma once
#include <bind/Registry.h>
#include <bind/util/meta.hpp>
#include <bind/PointerType.h>
#include <bind/FunctionType.h>
#include <utils/Exception.h>

namespace bind {
    template <typename T>
    DataType* Registry::GetType() {
        if (!instance) throw Exception("Registry::GetType - Registry has not been created");
        size_t hash = type_hash<T>();
        
        {
            std::shared_lock l(instance->m_mutex);
            auto it = instance->m_hostTypeMap.find(hash);
            if (it != instance->m_hostTypeMap.end()) return it->second;
        }

        if constexpr (std::is_pointer_v<T>) {
            DataType* tp = GetType<std::remove_pointer_t<T>>();
            if (!tp) {
                throw Exception(String::Format(
                    "Registry::GetType - Could not automatically register pointer type, base type '%s' has not been registered",
                    type_name<std::remove_pointer_t<T>>()
                ));
            }

            PointerType* pt = tp->getPointerType();
            instance->m_hostTypeMap.insert(std::pair<size_t, DataType*>(hash, pt));
            return pt;
        }
        
        if constexpr (std::is_reference_v<T>) {
            DataType* tp = GetType<std::remove_reference_t<T>>();
            if (!tp) {
                throw Exception(String::Format(
                    "Registry::GetType - Could not automatically register pointer type, base type '%s' has not been registered",
                    type_name<std::remove_reference_t<T>>()
                ));
            }

            PointerType* pt = tp->getPointerType();
            instance->m_hostTypeMap.insert(std::pair<size_t, DataType*>(hash, pt));
            return pt;
        }
        
        return nullptr;
    }

    template <typename Ret, typename... Args>
    FunctionType* Registry::Signature() {
        if (!instance) throw Exception("Registry::Signature - Registry has not been created");

        size_t hash = type_hash<Ret(*)(Args...)>();

        FunctionType* sig = nullptr;
        {
            std::shared_lock l(instance->m_mutex);
            auto it = instance->m_hostTypeMap.find(hash);
            if (it != instance->m_hostTypeMap.end()) {
                sig = (FunctionType*)it->second;
            }
        }

        if (sig) {
            if (sig->getInfo().is_function == 0) {
                throw Exception("Registry::Signature - Cached signature lookup returned type that is not a function type");
            }

            return sig;
        }

        DataType* retTp = GetType<Ret>();
        if (!retTp) {
            throw Exception(String::Format("Registry::Signature - Return type '%s' has not been registered", type_name<Ret>()));
        }

        const char* argTpNames[] = { type_name<Args>()..., nullptr };
        DataType* argTps[] = { GetType<Args>()..., nullptr };
        u32 argCount = u32(sizeof(argTps) / sizeof(DataType*)) - 1;

        String name = retTp->getFullName() + "(";
        
        for (u8 i = 0;i < argCount;i++) {
            if (!argTps[i]) {
                delete sig;
                throw Exception(String::Format("Registry::Signature - Type '%s' of argument %d has not been registered", argTpNames[i], i));
            }
            
            if (i > 0) name += ",";
            name += argTps[i]->getFullName();
        }

        name += ")";

        sig = new FunctionType(name, meta<Ret(*)(Args...)>());
        
        for (u8 i = 0;i < argCount;i++) {
            sig->m_args.push(FunctionType::Argument(i, argTps[i]));
        }

        sig->m_returnType = retTp;

        Add(sig, hash);

        sig->initCallInterface(FFI_DEFAULT_ABI);

        return sig;
    }

    template <typename Ret, typename Cls, typename... Args>
    FunctionType* Registry::MethodSignature() {
        if (!instance) throw Exception("Registry::MethodSignature - Registry has not been created");
        
        size_t hash = type_hash<Ret(Cls::*)(Args...)>();

        FunctionType* sig = nullptr;
        {
            std::shared_lock l(instance->m_mutex);
            auto it = instance->m_hostTypeMap.find(hash);
            if (it != instance->m_hostTypeMap.end()) {
                sig = (FunctionType*)it->second;
            }
        }

        if (sig) {
            if (sig->getInfo().is_function == 0) {
                throw Exception("Registry::Signature - Cached signature lookup returned type that is not a function type");
            }

            return sig;
        }

        DataType* retTp = GetType<Ret>();
        if (!retTp) {
            throw Exception(String::Format("Registry::MethodSignature - Return type '%s' has not been registered", type_name<Ret>()));
        }

        DataType* selfTp = GetType<Cls>();
        if (!selfTp) {
            throw Exception(String::Format("Registry::MethodSignature - Class type '%s' has not been registered", type_name<Ret>()));
        }

        const char* argTpNames[] = { type_name<Args>()..., nullptr };
        DataType* argTps[] = { GetType<Args>()..., nullptr };
        u32 argCount = u32(sizeof(argTps) / sizeof(DataType*)) - 1;

        String name = retTp->getFullName() + " " + selfTp->getFullName() + "::(";
        for (u8 i = 0;i < argCount;i++) {
            if (!argTps[i]) {
                delete sig;
                throw Exception(String::Format("Registry::MethodSignature - Type '%s' of argument %d has not been registered", argTpNames[i], i));
            }

            if (i > 0) name += ",";
            name += argTps[i]->getFullName();
        }
        
        name += ")";

        sig = new FunctionType(name, meta<Ret(Cls::*)(Args...)>());
        for (u8 i = 0;i < argCount;i++) {
            sig->m_args.push(FunctionType::Argument(i, argTps[i]));
        }
        
        sig->m_returnType = retTp;
        sig->m_thisType = selfTp;
        sig->m_wrapperAddress = &_method_wrapper<Cls, Ret, Args...>;
        
        Add(sig, hash);

        #if defined(X86_WIN64)
            sig->initCallInterface(FFI_DEFAULT_ABI);
        #elif defined(X86_64) || (defined (__x86_64__) && defined (X86_DARWIN))
            sig->initCallInterface(FFI_DEFAULT_ABI);
        #elif defined(X86_WIN32)
            sig->initCallInterface(FFI_THISCALL);
        #endif

        return sig;
    }
};