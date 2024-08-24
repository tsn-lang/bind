#pragma once
#include <bind/types.h>
#include <stddef.h>

namespace bind {
    class Pointer {
        public:
            class _undefinedClass;
            using FuncPtrTp = void (*)();
            using MethodPtrTp = void(_undefinedClass::*)();

            Pointer();
            Pointer(const Pointer& ptr);
            Pointer(void* ptr);
            Pointer(void* ptr, size_t size);

            template <typename Ret, typename... Args>
            Pointer(Ret (*ptr)(Args...));

            template <typename Cls, typename Ret, typename... Args>
            Pointer(Ret (Cls::*ptr)(Args...));

            template <typename Cls, typename Ret, typename... Args>
            Pointer(Ret (Cls::*ptr)(Args...) const);

            template <typename Ret, typename... Args>
            void get(Ret (**ptr)(Args...)) const;

            template <typename Cls, typename Ret, typename... Args>
            void get(Ret (Cls::**ptr)(Args...)) const;

            template <typename Cls, typename Ret, typename... Args>
            void get(Ret (Cls::**ptr)(Args...) const) const;

            void* get() const;

            void operator=(const Pointer& ptr);

        protected:
            u8 m_data[32];
    };
};