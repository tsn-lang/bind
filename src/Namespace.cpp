#include <bind/Namespace.h>
#include <bind/Registry.h>
#include <bind/AliasType.h>

namespace bind {
    Namespace::Namespace(const String& name)
        : ISymbol(
            name,
            ISymbol::genFullSymbolName(Registry::GlobalNamespace(), name),
            ISymbol::genNamespaceSymbolName(Registry::GlobalNamespace(), name),
            SymbolType::Namespace
        ), m_parent(nullptr)
    {
    }

    Namespace::Namespace(Namespace* parent, const String& name)
        : ISymbol(
            name,
            ISymbol::genFullSymbolName(parent, name),
            ISymbol::genNamespaceSymbolName(parent, name),
            SymbolType::Namespace
        ), m_parent(parent)
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
        auto it = m_symbolMap.find(ISymbol::genSymbolID(symbolName));
        if (it != m_symbolMap.end()) return it->second;

        return nullptr;
    }
};