#include <bind/Registry.h>
#include <bind/DataType.h>
#include <utils/Array.hpp>
#include <utils/Exception.h>
#include <mutex>

namespace bind {
    Registry* Registry::instance = nullptr;

    Registry::Registry() {
    }

    Registry::~Registry() {
        std::unique_lock l(instance->m_mutex);
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

    DataType* Registry::Get(u64 symHash) {
        if (!instance) throw Exception("Registry::Get - Registry has not been created");
        std::shared_lock l(instance->m_mutex);

        auto it = instance->m_typeMap.find(symHash);
        if (it != instance->m_typeMap.end()) return it->second;

        return nullptr;
    }
    
    std::shared_lock<std::shared_mutex> Registry::ReadLock() {
        if (!instance) throw Exception("Registry::ReadLock - Registry has not been created");
        return std::shared_lock(instance->m_mutex);
    }
};