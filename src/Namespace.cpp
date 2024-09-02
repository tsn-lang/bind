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
        ), m_parent(nullptr), m_forType(nullptr)
    {
    }

    Namespace::Namespace(Namespace* parent, const String& name)
        : ISymbol(
            name,
            ISymbol::genFullSymbolName(parent, name),
            ISymbol::genNamespaceSymbolName(parent, name),
            SymbolType::Namespace
        ), m_parent(parent), m_forType(nullptr)
    {
    }

    Namespace::Namespace(DataType* type)
        : ISymbol(
            type->getName(),
            ISymbol::genFullSymbolName(Registry::GlobalNamespace(), type->getName()),
            ISymbol::genNamespaceSymbolName(Registry::GlobalNamespace(), type->getName()),
            SymbolType::Namespace
        ), m_parent(nullptr), m_forType(type)
    {
    }

    Namespace::Namespace(Namespace* parent, DataType* type)
        : ISymbol(
            type->getName(),
            ISymbol::genFullSymbolName(parent, type->getName()),
            ISymbol::genNamespaceSymbolName(parent, type->getName()),
            SymbolType::Namespace
        ), m_parent(parent), m_forType(type)
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
    
    DataType* Namespace::getCorrespondingType() const {
        return m_forType;
    }

    ISymbol* Namespace::findSymbol(const String& symbolName) const {
        auto it = m_symbolMap.find(ISymbol::genSymbolID(symbolName));
        if (it != m_symbolMap.end()) return it->second;

        return nullptr;
    }
};