#include <bind/Registry.h>
#include <bind/DataType.h>
#include <bind/Function.h>
#include <bind/ValuePointer.h>
#include <bind/Namespace.h>
#include <utils/Array.hpp>
#include <utils/Exception.h>
#include <mutex>

namespace bind {
    Registry* Registry::instance = nullptr;

    Registry::Registry() {
        m_global = nullptr;
    }

    Registry::~Registry() {
        std::unique_lock l(instance->m_mutex);

        for (auto& it : instance->m_namespaceMap) delete it.second;
        for (auto& it : instance->m_valueMap) delete it.second;
        for (auto& it : instance->m_funcMap) delete it.second;
        for (auto& it : instance->m_typeMap) delete it.second;
        
        m_global = nullptr;
    }

    void Registry::Create() {
        if (instance) return;
        instance = new Registry();
        instance->m_global = new Namespace("");
        
        Registry::Add(instance->m_global);
    }
    
    void Registry::Reset() {
        if (!instance) throw Exception("Registry::Reset - Registry has not been created");
        Registry::Destroy();
        Registry::Create();
    }

    void Registry::Destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }

    void Registry::Add(DataType* tp) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(tp->getSymbolId());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Type '%s' already registered", tp->getFullName().c_str()));
        }

        instance->m_typeMap.insert(std::pair<symbol_id, DataType*>(tp->getSymbolId(), tp));
    }

    void Registry::Add(DataType* tp, size_t nativeHash) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(tp->getSymbolId());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Type '%s' already registered", tp->getFullName().c_str()));
        }

        instance->m_typeMap.insert(std::pair<symbol_id, DataType*>(tp->getSymbolId(), tp));
        instance->m_hostTypeMap.insert(std::pair<size_t, DataType*>(nativeHash, tp));
    }

    void Registry::Add(Function* fn) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(fn->getSymbolId());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Function '%s' already registered", fn->getFullName().c_str()));
        }

        instance->m_funcMap.insert(std::pair<symbol_id, Function*>(fn->getSymbolId(), fn));
    }

    void Registry::Add(ValuePointer* val) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(val->getSymbolId());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Function '%s' already registered", val->getFullName().c_str()));
        }

        instance->m_valueMap.insert(std::pair<symbol_id, ValuePointer*>(val->getSymbolId(), val));
    }

    void Registry::Add(Namespace* ns) {
        if (!instance) throw Exception("Registry::Add - Registry has not been created");

        std::unique_lock l(instance->m_mutex);
        auto it = instance->m_typeMap.find(ns->getSymbolId());
        if (it != instance->m_typeMap.end()) {
            throw Exception(String::Format("Registry::Add - Function '%s' already registered", ns->getFullName().c_str()));
        }

        instance->m_namespaceMap.insert(std::pair<symbol_id, Namespace*>(ns->getSymbolId(), ns));
    }

    DataType* Registry::GetType(symbol_id id) {
        if (!instance) throw Exception("Registry::GetType - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_typeMap.find(id);
        if (it != instance->m_typeMap.end()) return it->second;

        return nullptr;
    }

    Function* Registry::GetFunc(symbol_id id) {
        if (!instance) throw Exception("Registry::GetFunc - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_funcMap.find(id);
        if (it != instance->m_funcMap.end()) return it->second;

        return nullptr;
    }

    ValuePointer* Registry::GetValue(symbol_id id) {
        if (!instance) throw Exception("Registry::GetValue - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_valueMap.find(id);
        if (it != instance->m_valueMap.end()) return it->second;

        return nullptr;
    }

    Namespace* Registry::GetNamespace(symbol_id id) {
        if (!instance) throw Exception("Registry::GetNamespace - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_namespaceMap.find(id);
        if (it != instance->m_namespaceMap.end()) return it->second;

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

    Array<ValuePointer*> Registry::Values() {
        if (!instance) throw Exception("Registry::Values - Registry has not been created");
        Array<ValuePointer*> ret;

        for (auto& it : instance->m_valueMap) ret.push(it.second);

        return ret;
    }
    
    Namespace* Registry::GlobalNamespace() {
        if (!instance) throw Exception("Registry::GlobalNamespace - Registry has not been created");
        return instance->m_global;
    }
};