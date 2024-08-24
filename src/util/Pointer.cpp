#include <bind/util/Pointer.h>

namespace bind {
    Pointer::Pointer() {
        ((u64*)m_data)[0] = 0;
        ((u64*)m_data)[1] = 0;
        ((u64*)m_data)[2] = 0;
        ((u64*)m_data)[3] = 0;
    }

    Pointer::Pointer(void* ptr) {
        if constexpr (sizeof(void*) == sizeof(u64)) {
            ((void**)m_data)[0] = ptr;
            ((u64*)m_data)[1] = 0;
            ((u64*)m_data)[2] = 0;
            ((u64*)m_data)[3] = 0;
        } else {
            ((u64*)m_data)[0] = 0;
            ((u64*)m_data)[1] = 0;
            ((u64*)m_data)[2] = 0;
            ((u64*)m_data)[3] = 0;
            ((void**)m_data)[0] = ptr;
        }
    }
    
    Pointer::Pointer(const Pointer& ptr) {
        ((u64*)m_data)[0] = ((u64*)ptr.m_data)[0];
        ((u64*)m_data)[1] = ((u64*)ptr.m_data)[1];
        ((u64*)m_data)[2] = ((u64*)ptr.m_data)[2];
        ((u64*)m_data)[3] = ((u64*)ptr.m_data)[3];
    }

    Pointer::Pointer(void* ptr, size_t size) {
        ((u64*)m_data)[0] = 0;
        ((u64*)m_data)[1] = 0;
        ((u64*)m_data)[2] = 0;
        ((u64*)m_data)[3] = 0;
        for (u32 i = 0;i < size;i++) m_data[i] = ((u8*)ptr)[i];
    }

    void* Pointer::get() const {
        return ((void**)m_data)[0];
    }
    
    void Pointer::operator=(const Pointer& ptr) {
        ((u64*)m_data)[0] = ((u64*)ptr.m_data)[0];
        ((u64*)m_data)[1] = ((u64*)ptr.m_data)[1];
        ((u64*)m_data)[2] = ((u64*)ptr.m_data)[2];
        ((u64*)m_data)[3] = ((u64*)ptr.m_data)[3];
    }
};