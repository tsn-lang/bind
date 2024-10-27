#pragma once
#include <bind/types.h>
#include <utils/Array.h>
#include <shared_mutex>
#include <unordered_map>

namespace bind {
    class DataType;
    class FunctionType;
    class Function;
    class ValuePointer;
    class Namespace;

    class Registry {
        public:
            static void Create();
            static void Reset();
            static void Destroy();

            static void Add(DataType* tp);
            static void Add(DataType* tp, size_t nativeHash);
            static void Add(Function* fn);
            static void Add(ValuePointer* val);
            static void Add(Namespace* ns);
            static void Remove(DataType* tp);
            static void Remove(Function* fn);
            static void Remove(ValuePointer* val);
            static void Remove(Namespace* ns);
            static DataType* GetType(symbol_id id);
            static Function* GetFunc(symbol_id id);
            static ValuePointer* GetValue(symbol_id id);
            static Namespace* GetNamespace(symbol_id id);
            static std::shared_lock<std::shared_mutex> ReadLock();
            static Array<DataType*> Types();
            static Array<Function*> Functions();
            static Array<ValuePointer*> Values();
            static Namespace* GlobalNamespace();
            
            static FunctionType* Signature(DataType* returnType, const Array<DataType*>& args);
            static FunctionType* Signature(DataType* returnType, DataType** args, u32 argCount, bool* didExist = nullptr);
            static FunctionType* MethodSignature(DataType* returnType, DataType* thisType, const Array<DataType*>& args);
            static FunctionType* MethodSignature(DataType* returnType, DataType* thisType, DataType** args, u32 argCount, bool* didExist = nullptr);

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
            std::unordered_map<symbol_id, DataType*> m_typeMap;
            std::unordered_map<size_t, DataType*> m_hostTypeMap;
            std::unordered_map<symbol_id, Function*> m_funcMap;
            std::unordered_map<symbol_id, ValuePointer*> m_valueMap;
            std::unordered_map<symbol_id, Namespace*> m_namespaceMap;

            Namespace* m_global;
    };
};