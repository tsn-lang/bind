#include <bind/Function.h>

namespace bind {
    Function::Function(const String& name, FunctionType* sig, Namespace* ns)
        : ISymbol(name, ISymbol::genFullSymbolName(ns, name), ISymbol::genFuncSymbolName(ns, name, sig), SymbolType::Function),
          m_signature(sig), m_callHandler(nullptr)
    {
    }
    Function::Function(const String& name, const Pointer& address, FunctionType* sig, Namespace* ns) 
        : ISymbol(name, ISymbol::genFullSymbolName(ns, name), ISymbol::genFuncSymbolName(ns, name, sig), SymbolType::Function),
          m_signature(sig), m_callHandler(nullptr), m_address(address)
    {
    }

    Function::~Function() {
    }

    void Function::setCallHandler(ICallHandler* callHandler) {
        m_callHandler = callHandler;
    }

    FunctionType* Function::getSignature() const {
        return m_signature;
    }

    ICallHandler* Function::getCallHandler() const {
        return m_callHandler;
    }

    const Pointer& Function::getAddress() const {
        return m_address;
    }
}