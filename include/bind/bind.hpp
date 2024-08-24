#pragma once
#include <bind/bind.h>

namespace bind {
    template <typename T>
    std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
    build(const String& name) {
        DataType* tp = Registry::Get<T>();
        if (tp) throw Exception(String::Format("build - Type '%s' has already been registered", type_name<T>()));

        return ObjectTypeBuilder<T>(name, nullptr);
    }

    template <typename T>
    std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
    extend() {
        DataType* tp = Registry::Get<T>();
        if (!tp) throw Exception(String::Format("extend - Type '%s' has not been registered", type_name<T>()));

        return ObjectTypeBuilder<T>(tp);
    }

    template <typename T>
    std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
    build(const String& name) {
        DataType* tp = Registry::Get<T>();
        if (tp) throw Exception(String::Format("build - Type '%s' has already been registered", type_name<T>()));

        return PrimitiveTypeBuilder<T>(name, nullptr);
    }

    template <typename T>
    std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
    extend() {
        DataType* tp = Registry::Get<T>();
        if (!tp) throw Exception(String::Format("extend - Type '%s' has not been registered", type_name<T>()));

        return PrimitiveTypeBuilder<T>(tp);
    }

    template <typename T>
    std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
    build(const String& name) {
        DataType* tp = Registry::Get<T>();
        if (tp) throw Exception(String::Format("build - Type '%s' has already been registered", type_name<T>()));

        return EnumTypeBuilder<T>(name, nullptr);
    }

    template <typename T>
    std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
    extend() {
        DataType* tp = Registry::Get<T>();
        if (!tp) throw Exception(String::Format("extend - Type '%s' has not been registered", type_name<T>()));

        return EnumTypeBuilder<T>(tp);
    }

    template <typename T>
    AliasType* alias(const String& name) {
        DataType* tp = Registry::Get<T>();
        if (!tp) throw Exception(String::Format("alias - Type '%s' has not been registered", type_name<T>()));

        return alias(name, tp);
    }
};