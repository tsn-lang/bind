#include <bind/Function.h>

namespace bind {
    Function::Function(const String& name, FunctionType* sig, Namespace* ns)
        : ISymbol(name, ISymbol::genFullSymbolName(ns, name), ISymbol::genFuncSymbolName(ns, name, sig), SymbolType::Function),
          m_signature(sig)
    {
    }
    Function::Function(const String& name, const Pointer& address, FunctionType* sig, Namespace* ns) 
        : ISymbol(name, ISymbol::genFullSymbolName(ns, name), ISymbol::genFuncSymbolName(ns, name, sig), SymbolType::Function),
          m_signature(sig), m_address(address)
    {
    }

    FunctionType* Function::getSignature() const {
        return m_signature;
    }

    const Pointer& Function::getAddress() const {
        return m_address;
    }
}