#pragma once
#include <bind/Namespace.h>

namespace bind {
    template <typename T>
    std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
    Namespace::build(const String& name) {
        DataType* tp = Registry::GetType<T>();
        if (tp) throw Exception(String::Format("Namespace::build - Type '%s' has already been registered", type_name<T>()));

        ObjectTypeBuilder<T> ret = ObjectTypeBuilder<T>(name, this);
        m_symbolMap.insert(std::pair<u64, ISymbol*>(ret.getType()->getSymbolId(), ret.getType()));
        return ret;
    }

    template <typename T>
    std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
    Namespace::extend() {
        DataType* tp = Registry::GetType<T>();
        if (!tp) throw Exception(String::Format("Namespace::extend - Type '%s' has not been registered", type_name<T>()));

        return ObjectTypeBuilder<T>(tp);
    }

    template <typename T>
    std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
    Namespace::build(const String& name) {
        DataType* tp = Registry::GetType<T>();
        if (tp) throw Exception(String::Format("Namespace::build - Type '%s' has already been registered", type_name<T>()));

        PrimitiveTypeBuilder<T> ret = PrimitiveTypeBuilder<T>(name, this);
        m_symbolMap.insert(std::pair<u64, ISymbol*>(ret.getType()->getSymbolId(), ret.getType()));
        return ret;
    }

    template <typename T>
    std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
    Namespace::extend() {
        DataType* tp = Registry::GetType<T>();
        if (!tp) throw Exception(String::Format("Namespace::extend - Type '%s' has not been registered", type_name<T>()));

        return PrimitiveTypeBuilder<T>(tp);
    }

    template <typename T>
    std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
    Namespace::build(const String& name) {
        DataType* tp = Registry::GetType<T>();
        if (tp) throw Exception(String::Format("Namespace::build - Type '%s' has already been registered", type_name<T>()));

        return EnumTypeBuilder<T>(name, this);
    }

    template <typename T>
    std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
    Namespace::extend() {
        DataType* tp = Registry::GetType<T>();
        if (!tp) throw Exception(String::Format("Namespace::extend - Type '%s' has not been registered", type_name<T>()));

        return EnumTypeBuilder<T>(tp);
    }
    
    template <typename T>
    ValuePointer* Namespace::value(const String& name, T* val) {
        DataType* tp = Registry::GetType<T>();
        if (!tp) throw Exception(String::Format("Namespace::value - Type '%s' has not been registered", type_name<T>()));

        ValuePointer v = new ValuePointer(name, tp, val, this);
        Registry::Add(v);
        return v;
    }

    template <typename Ret, typename... Args>
    Function* Namespace::function(const String& name, Ret (*fn)(Args...)) {
        Function* func = new Function(
            name,
            fn,
            Registry::Signature<Ret, Args...>(),
            this
        );

        func->setCallHandler(HostCallHandler::get());

        Registry::Add(func);
        return func;
    }

    template <typename T>
    AliasType* Namespace::alias(const String& name) {
        DataType* tp = Registry::GetType<T>();
        if (!tp) throw Exception(String::Format("Namespace::alias - Type '%s' has not been registered", type_name<T>()));

        AliasType* ret = Namespace::alias(name, tp);
        m_symbolMap.insert(std::pair<u64, ISymbol*>(ret->getSymbolId(), ret));
        return ret;
    }
}