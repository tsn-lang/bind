#pragma once
#include <bind/types.h>
#include <utils/Array.h>
#include <shared_mutex>
#include <unordered_map>

namespace bind {
    class DataType;
    class FunctionType;

    class Registry {
        public:
            static void Create();
            static void Destroy();

            static void Add(DataType* tp);
            static void Add(DataType* tp, size_t nativeHash);
            static DataType* Get(u64 symHash);
            static std::shared_lock<std::shared_mutex> ReadLock();
            static const Array<DataType*>& All();

            template <typename T>
            static DataType* Get();

            template <typename Ret, typename... Args>
            static FunctionType* Signature();

            template <typename Ret, typename Cls, typename... Args>
            static FunctionType* MethodSignature();

        protected:
            static Registry* instance;

            Registry();
            ~Registry();

            std::shared_mutex m_mutex;
            std::unordered_map<u64, DataType*> m_typeMap;
            std::unordered_map<size_t, DataType*> m_hostTypeMap;
    };
};