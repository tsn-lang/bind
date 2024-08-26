#include <bind/Registry.h>
#include <bind/DataType.h>
#include <bind/Function.h>
#include <bind/Value.h>
#include <bind/Namespace.h>
#include <bind/util/Array.hpp>
#include <bind/util/Exception.h>
#include <mutex>

namespace bind {
    Registry* Registry::instance = nullptr;

    Registry::Registry() {
        m_global = new Namespace("");
    }

    Registry::~Registry() {
        std::unique_lock l(instance->m_mutex);

        for (auto& it : instance->m_valueMap) delete it.second;
        for (auto& it : instance->m_funcMap) delete it.second;
        for (auto& it : instance->m_typeMap) delete it.second;
        
        delete m_global;
        m_global = nullptr;
    }

    void Registry::Create() {
        if (instance) return;
        instance = new Registry();
    }

    void Registry::Destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }

    void Registry::Add(DataType* tp) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(tp->getSymbolHash());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Type '%s' already registered", tp->getName().c_str()));
        }

        instance->m_typeMap.insert(std::pair<u64, DataType*>(tp->getSymbolHash(), tp));
    }

    void Registry::Add(DataType* tp, size_t nativeHash) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(tp->getSymbolHash());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Type '%s' already registered", tp->getName().c_str()));
        }

        instance->m_typeMap.insert(std::pair<u64, DataType*>(tp->getSymbolHash(), tp));
        instance->m_hostTypeMap.insert(std::pair<size_t, DataType*>(nativeHash, tp));
    }

    void Registry::Add(Function* fn) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(fn->getSymbolHash());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Function '%s' already registered", fn->getName().c_str()));
        }

        instance->m_funcMap.insert(std::pair<u64, Function*>(fn->getSymbolHash(), fn));
    }

    void Registry::Add(Value* val) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(val->getSymbolHash());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Function '%s' already registered", val->getName().c_str()));
        }

        instance->m_valueMap.insert(std::pair<u64, Value*>(val->getSymbolHash(), val));
    }

    DataType* Registry::GetType(u64 symHash) {
        if (!instance) throw Exception("Registry::GetType - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_typeMap.find(symHash);
        if (it != instance->m_typeMap.end()) return it->second;

        return nullptr;
    }

    Function* Registry::GetFunc(u64 symHash) {
        if (!instance) throw Exception("Registry::GetFunc - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_funcMap.find(symHash);
        if (it != instance->m_funcMap.end()) return it->second;

        return nullptr;
    }

    Value* Registry::GetValue(u64 symHash) {
        if (!instance) throw Exception("Registry::GetValue - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_valueMap.find(symHash);
        if (it != instance->m_valueMap.end()) return it->second;

        return nullptr;
    }
    
    std::shared_lock<std::shared_mutex> Registry::ReadLock() {
        if (!instance) throw Exception("Registry::ReadLock - Registry has not been created");
        return std::shared_lock(instance->m_mutex);
    }

    Array<DataType*> Registry::Types() {
        if (!instance) throw Exception("Registry::Types - Registry has not been created");
        Array<DataType*> ret;

        for (auto& it : instance->m_typeMap) ret.push(it.second);

        return ret;
    }

    Array<Function*> Registry::Functions() {
        if (!instance) throw Exception("Registry::Functions - Registry has not been created");
        Array<Function*> ret;

        for (auto& it : instance->m_funcMap) ret.push(it.second);

        return ret;
    }

    Array<Value*> Registry::Values() {
        if (!instance) throw Exception("Registry::Values - Registry has not been created");
        Array<Value*> ret;

        for (auto& it : instance->m_valueMap) ret.push(it.second);

        return ret;
    }
    
    Namespace* Registry::GlobalNamespace() {
        if (!instance) throw Exception("Registry::GlobalNamespace - Registry has not been created");
        return instance->m_global;
    }
};