#pragma once
#include <utils/types.h>

namespace bind {
    using namespace utils;
    
    struct type_meta {
        unsigned size : 16;
        unsigned is_pod : 1;
        unsigned is_trivially_constructible : 1;
        unsigned is_trivially_copyable : 1;
        unsigned is_trivially_destructible : 1;
        unsigned is_primitive : 1;
        unsigned is_floating_point : 1;
        unsigned is_integral : 1;
        unsigned is_unsigned : 1;
        unsigned is_function : 1;
        unsigned is_pointer : 1;
        unsigned is_alias : 1;
        unsigned is_enum : 1;
        unsigned __pad0 : 4;
    };
};