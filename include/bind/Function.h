#pragma once
#include <bind/interfaces/ISymbol.h>
#include <bind/interfaces/IWithUserData.h>
#include <bind/util/Pointer.h>

namespace bind {
    class Function : public ISymbol, public IWithFixedUserData<32> {
        public:
            Function(const String& name, FunctionType* sig, Namespace* ns);
            Function(const String& name, const Pointer& address, FunctionType* sig, Namespace* ns);

            FunctionType* getSignature() const;
            const Pointer& getAddress() const;
        
        protected:
            FunctionType* m_signature;
            Pointer m_address;
    };
};