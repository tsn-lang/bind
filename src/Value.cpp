#include <bind/Value.h>

namespace bind {
    Value::Value(const String& name, DataType* type, void* address, Namespace* ns)
        : ISymbol(name, ISymbol::genValueSymbolName(ns, name), SymbolType::Value),
          m_type(type), m_address(address)
    {
    }

    Value::~Value() {
    }

    DataType* Value::getType() const {
        return m_type;
    }

    void* Value::getAddress() const {
        return m_address;
    }
};