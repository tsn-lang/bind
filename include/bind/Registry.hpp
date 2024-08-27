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

            PointerType* pt = new PointerType(tp);
            Add(pt, hash);
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

            PointerType* pt = new PointerType(tp);
            Add(pt, hash);
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
            auto it = instance->m_typeMap.find(hash);
            if (it != instance->m_typeMap.end()) {
                DataType* ret = it->second;
                if (ret->getInfo().is_function == 0) {
                    l.unlock();
                    throw Exception("Registry::Signature - Cached signature lookup returned type that is not a function type");
                }

                return (FunctionType*)ret;
            }

            DataType* retTp = GetType<Ret>();
            if (!retTp) {
                l.unlock();
                throw Exception(String::Format("Registry::Signature - Return type '%s' has not been registered", type_name<Ret>()));
            }

            const char* argTpNames[] = { type_name<Args>()... };
            DataType* argTps[] = { GetType<Args>()... };
            u8 argCount = u8(sizeof(argTps) / sizeof(DataType*));

            String name = retTp->getName() + "(";
            
            for (u8 i = 0;i < argCount;i++) {
                if (!argTps[i]) {
                    delete sig;
                    l.unlock();
                    throw Exception(String::Format("Registry::Signature - Type '%s' of argument %d has not been registered", argTpNames[i], i));
                }
                
                if (i > 0) name += ",";
                name += argTps[i]->getName();
            }

            name += ")";

            sig = new FunctionType(name, meta<Ret(*)(Args...)>());
            
            for (u8 i = 0;i < argCount;i++) {
                sig->m_args.push(FunctionType::Argument(i, argTps[i]));
            }

            sig->m_returnType = retTp;
        }
        
        Add(sig);

        return sig;
    }

    template <typename Ret, typename Cls, typename... Args>
    FunctionType* Registry::MethodSignature() {
        if (!instance) throw Exception("Registry::MethodSignature - Registry has not been created");
        
        size_t hash = type_hash<Ret(Cls::*)(Args...)>();

        FunctionType* sig = nullptr;
        {
            std::shared_lock l(instance->m_mutex);
            auto it = instance->m_typeMap.find(hash);
            if (it != instance->m_typeMap.end()) {
                DataType* ret = it->second;
                if (ret->getInfo().is_function == 0) {
                    l.unlock();
                    throw Exception("Registry::Signature - Cached signature lookup returned type that is not a function type");
                }

                return (FunctionType*)ret;
            }

            DataType* retTp = GetType<Ret>();
            if (!retTp) {
                l.unlock();
                throw Exception(String::Format("Registry::MethodSignature - Return type '%s' has not been registered", type_name<Ret>()));
            }

            DataType* selfTp = GetType<Cls>();
            if (!selfTp) {
                l.unlock();
                throw Exception(String::Format("Registry::MethodSignature - Class type '%s' has not been registered", type_name<Ret>()));
            }

            const char* argTpNames[] = { type_name<Args>()... };
            DataType* argTps[] = { GetType<Args>()... };
            u32 argCount = sizeof(argTps) / sizeof(DataType*);

            String name = retTp->getName() + " " + selfTp->getName() + "::(";
            for (u8 i = 0;i < argCount;i++) {
                if (!argTps[i]) {
                    delete sig;
                    l.unlock();
                    throw Exception(String::Format("Registry::MethodSignature - Type '%s' of argument %d has not been registered", argTpNames[i], i));
                }

                if (i > 0) name += ",";
                name += argTps[i]->getName();
            }
            
            name += ")";

            sig = new FunctionType(name, meta<Ret(*)(Args...)>());
            for (u8 i = 0;i < argCount;i++) {
                sig->m_args.push(FunctionType::Argument(i, argTps[i]));
            }
        }
        
        Add(sig);

        return sig;
    }
};