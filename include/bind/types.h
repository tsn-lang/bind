#pragma once
#include <stdint.h>
#ifndef _MSC_VER
#include <stddef.h>
#endif

namespace bind {
    typedef uint64_t    u64;
    typedef int64_t     i64;
    typedef uint32_t    u32;
    typedef int32_t     i32;
    typedef uint16_t    u16;
    typedef int16_t     i16;
    typedef uint8_t     u8;
    typedef int8_t      i8;
    typedef float       f32;
    typedef double      f64;
    typedef i32         type_id;

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