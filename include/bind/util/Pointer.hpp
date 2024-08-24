#pragma once
#include <bind/util/Pointer.h>

namespace bind {
    template <typename Ret, typename... Args>
    Pointer::Pointer(Ret (*ptr)(Args...)) : Pointer(reinterpret_cast<void*>(&ptr), sizeof(ptr)) {}

    template <typename Cls, typename Ret, typename... Args>
    Pointer::Pointer(Ret (Cls::*ptr)(Args...)) : Pointer(reinterpret_cast<void*>(&ptr), sizeof(ptr)) {}

    template <typename Cls, typename Ret, typename... Args>
    Pointer::Pointer(Ret (Cls::*ptr)(Args...) const) : Pointer(reinterpret_cast<void*>(&ptr), sizeof(ptr)) {}

    template <typename Ret, typename... Args>
    void Pointer::get(Ret (**ptr)(Args...)) const {
        constexpr u32 sz = sizeof(Ret(*)(Args...));

        for (u32 i = 0;i < sz;i++) ((u8*)ptr)[i] = m_data[i];
    }

    template <typename Cls, typename Ret, typename... Args>
    void Pointer::get(Ret (Cls::**ptr)(Args...)) const {
        constexpr u32 sz = sizeof(Ret(Cls::*)(Args...));

        for (u32 i = 0;i < sz;i++) ((u8*)ptr)[i] = m_data[i];
    }

    template <typename Cls, typename Ret, typename... Args>
    void Pointer::get(Ret (Cls::**ptr)(Args...) const) const {
        constexpr u32 sz = sizeof(Ret(Cls::*)(Args...) const);

        for (u32 i = 0;i < sz;i++) ((u8*)ptr)[i] = m_data[i];
    }
};