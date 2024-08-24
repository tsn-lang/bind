#include <bind/Namespace.h>
#include <bind/Registry.h>
#include <bind/AliasType.h>

namespace bind {
    Namespace::Namespace(const String& name)
        : ISymbol(name, ISymbol::genNamespaceSymbolName(nullptr, name), SymbolType::Namespace)
    {
    }

    Namespace::Namespace(Namespace* parent, const String& name)
        : ISymbol(name, ISymbol::genNamespaceSymbolName(parent, name), SymbolType::Namespace)
    {
    }

    AliasType* Namespace::alias(const String& name, DataType* aliasOf) {
        AliasType* tp = new AliasType(name, aliasOf, this);
        Registry::Add(tp);
        return tp;
    }

    const String& Namespace::getName() const {
        return m_name;
    }

    Namespace* Namespace::getParent() const {
        return m_parent;
    }

    ISymbol* Namespace::findSymbol(const String& symbolName) const {
        auto it = m_symbolMap.find(ISymbol::Hash(symbolName));
        if (it != m_symbolMap.end()) return it->second;

        return nullptr;
    }
};