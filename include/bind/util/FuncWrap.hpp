#pragma once

namespace bind {
    template <typename Cls, typename... Args>
    void __cdecl _constructor_wrapper(Cls* self, Args... args) {
        new (self) Cls (args...);
    }

    template <typename Cls>
    void __cdecl _destructor_wrapper(Cls* self) {
        self->~Cls();
    }
};