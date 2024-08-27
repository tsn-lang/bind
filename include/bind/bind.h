#pragma once
#include <bind/DataType.h>
#include <bind/FunctionType.h>
#include <bind/AliasType.h>
#include <bind/PointerType.h>
#include <bind/Namespace.hpp>
#include <bind/Registry.hpp>
#include <bind/util/ObjectTypeBuilder.hpp>
#include <bind/util/PrimitiveTypeBuilder.hpp>
#include <bind/util/EnumTypeBuilder.hpp>
#include <utils/Array.hpp>
#include <utils/Exception.h>
#include <utils/interfaces/IWithUserData.hpp>

namespace bind {
    template <typename T>
    std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
    build(const String& name);

    template <typename T>
    std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
    extend();

    template <typename T>
    std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
    build(const String& name);

    template <typename T>
    std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
    extend();

    template <typename T>
    std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
    build(const String& name);
    
    template <typename T>
    std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
    extend();

    template <typename T>
    AliasType* alias(const String& name);

    AliasType* alias(const String& name, DataType* aliasOf);
};

#include <bind/bind.hpp>