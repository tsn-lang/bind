#pragma once
#include <bind/util/Array.h>

namespace bind {
    template <typename T>
    Array<T>::Array() : m_size(0), m_capacity(0), m_data(nullptr) {
    }

    template <typename T>
    Array<T>::Array(std::initializer_list<T> list) {
        m_size = (u32)list.size();
        m_capacity = m_size;
        m_data = (T*)new u8[sizeof(T) * m_size];

        u32 i = 0;
        for (auto& ele : list) {
            new (((u8*)m_data) + (i * sizeof(T))) T(ele);
            i++;
        }
    }

    template <typename T>
    Array<T>::Array(const std::vector<T>& vec) {
        m_size = (u32)vec.size();
        m_capacity = m_size;
        m_data = (T*)new u8[sizeof(T) * m_size];

        const T* data = vec.data();
        for (u32 i = 0;i < m_size;i++) {
            new ((((u8*)m_data) + (i * sizeof(T)))) T(data[i]);
        }
    }

    template <typename T>
    Array<T>::~Array() {
        if (m_data) {
            for (u32 i = 0;i < m_size;i++) {
                m_data[i].~T();
            }

            delete [] (u8*)m_data;
            m_data = nullptr;
        }

        m_size = 0;
        m_capacity = 0;
    }

    template <typename T>
    u32 Array<T>::size() const {
        return m_size;
    }

    template <typename T>
    T* Array<T>::data() const {
        return m_data;
    }
    
    template <typename T>
    T& Array<T>::last() {
        return m_data[m_size - 1];
    }
    
    template <typename T>
    const T& Array<T>::last() const {
        return m_data[m_size - 1];
    }

    template <typename T>
    T& Array<T>::operator[](u32 idx) {
        return m_data[idx];
    }

    template <typename T>
    const T& Array<T>::operator[](u32 idx) const {
        return m_data[idx];
    }

    template <typename T>
    Array<T>::operator std::vector<T>() const {
        std::vector<T> ret;
        ret.reserve(m_size);

        for (u32 i = 0;i < m_size;i++) {
            ret.push_back(m_data[i]);
        }

        return ret;
    }

    template <typename T>
    void Array<T>::push(const T& elem) {
        if (m_capacity == 0) reserve(16);
        else if (m_size == m_capacity) reserve(m_capacity / 2);

        new (((u8*)m_data) + (m_size * sizeof(T))) T(elem);
        m_size++;
    }

    template <typename T>
    void Array<T>::pop() {
        if (m_size == 0) return;
        m_data[m_size--].~T();
    }

    template <typename T>
    void Array<T>::reserve(u32 count) {
        if (count == 0) return;

        u32 avail = m_capacity - m_size;
        if (avail > count) return;

        u32 diff = count - avail;
        m_capacity += diff;
        u8* newData = new u8[sizeof(T) * m_capacity];

        if (m_data) {
            for (u32 i = 0;i < m_size;i++) {
                new (newData + (i * sizeof(T))) T(m_data[i]);
                m_data[i].~T();
            }

            delete [] m_data;
        }

        m_data = (T*)newData;
    }
};