#include <bind/ValuePointer.h>

namespace bind {
    ValuePointer::ValuePointer(const String& name, DataType* type, void* address, Namespace* ns)
        : ISymbol(name, ISymbol::genValueSymbolName(ns, name), SymbolType::Value),
          m_type(type), m_address(address)
    {
    }

    ValuePointer::~ValuePointer() {
    }

    DataType* ValuePointer::getType() const {
        return m_type;
    }

    void* ValuePointer::getAddress() const {
        return m_address;
    }
};