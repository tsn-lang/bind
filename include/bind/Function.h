#pragma once
#include <bind/interfaces/ISymbol.h>
#include <utils/interfaces/IWithUserData.h>
#include <utils/Pointer.h>

namespace bind {
    class ICallHandler;

    class Function : public ISymbol, public IWithFixedUserData<32> {
        public:
            Function(const String& name, FunctionType* sig, Namespace* ns);
            Function(const String& name, const Pointer& address, FunctionType* sig, Namespace* ns);
            virtual ~Function();

            void setCallHandler(ICallHandler* callHandler);
            void call(void* retDest, void** args);

            FunctionType* getSignature() const;
            ICallHandler* getCallHandler() const;
            const Pointer& getAddress() const;
        
        protected:
            FunctionType* m_signature;
            ICallHandler* m_callHandler;
            Pointer m_address;
    };
};