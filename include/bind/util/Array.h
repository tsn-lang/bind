#pragma once
#include <bind/types.h>
#include <initializer_list>
#include <vector>

namespace bind {
    template <typename T>
    class Array {
        public:
            Array();
            Array(std::initializer_list<T> list);
            Array(const std::vector<T>& vec);
            ~Array();

            u32 size() const;
            T* data() const;
            T& last();
            const T& last() const;

            T& operator[](u32 idx);
            const T& operator[](u32 idx) const;
            operator std::vector<T>() const;

            void push(const T& elem);
            void pop();
            void reserve(u32 count);
        
        protected:
            u32 m_size;
            u32 m_capacity;
            T* m_data;
    };
};