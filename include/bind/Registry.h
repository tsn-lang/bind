#pragma once
#include <bind/types.h>
#include <bind/util/Array.h>
#include <shared_mutex>
#include <unordered_map>

namespace bind {
    class DataType;
    class FunctionType;
    class Function;
    class Value;
    class Namespace;

    class Registry {
        public:
            static void Create();
            static void Destroy();

            static void Add(DataType* tp);
            static void Add(DataType* tp, size_t nativeHash);
            static void Add(Function* fn);
            static void Add(Value* val);
            static DataType* GetType(u64 symHash);
            static Function* GetFunc(u64 symHash);
            static Value* GetValue(u64 symHash);
            static std::shared_lock<std::shared_mutex> ReadLock();
            static Array<DataType*> Types();
            static Array<Function*> Functions();
            static Array<Value*> Values();
            static Namespace* GlobalNamespace();

            template <typename T>
            static DataType* GetType();

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
            std::unordered_map<u64, Function*> m_funcMap;
            std::unordered_map<u64, Value*> m_valueMap;

            Namespace* m_global;
    };
};