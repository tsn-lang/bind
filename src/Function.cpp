#include <bind/Function.h>
#include <bind/interfaces/ICallHandler.h>
#include <utils/Exception.h>

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
    
    void Function::call(void* retDest, void** args) {
        if (!m_callHandler) {
            throw Exception("Function::call - function '%s' has no call handler", getFullName().c_str());
        }

        m_callHandler->call(this, retDest, args);
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