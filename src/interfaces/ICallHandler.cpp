#include <bind/interfaces/ICallHandler.h>
#include <bind/Function.h>
#include <bind/FunctionType.h>

namespace bind {
    HostCallHandler HostCallHandler::instance = HostCallHandler();

    void HostCallHandler::call(Function* target, void* retDest, void** args) {
        ffi_call(
            target->getSignature()->getCif(),
            (void(*)())target->getAddress().get(),
            retDest,
            args
        );
    }

    HostCallHandler* HostCallHandler::get() {
        return &HostCallHandler::instance;
    }

    HostThisCallHandler HostThisCallHandler::instance = HostThisCallHandler();

    void HostThisCallHandler::call(Function* target, void* retDest, void** args) {
        FunctionType* sig = target->getSignature();
        u32 argc = sig->getArgs().size();
        void* finalArgs[32];

        // wrapper signature: (method pointer, instance pointer, arguments...)
        finalArgs[0] = &target; // set first argument to target function, the wrapper needs it
        finalArgs[1] = args[0]; // first argument should be 'this' pointer, it's implicit
        for (u32 i = 0;i < argc;i++) finalArgs[i + 2] = args[i + 1];

        ffi_call(
            target->getSignature()->getCif(),
            (void(*)())sig->getWrapperAddress().get(),
            retDest,
            finalArgs
        );
    }

    HostThisCallHandler* HostThisCallHandler::get() {
        return &HostThisCallHandler::instance;
    }
};