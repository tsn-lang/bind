#include <bind/interfaces/ISymbol.h>
#include <bind/Namespace.h>
#include <bind/FunctionType.h>
#include <utils/Array.hpp>
#include <unordered_map>

namespace bind {
    ISymbol::ISymbol(const String& name, const String& symName, SymbolType type)
        : m_name(name), m_symName(symName), m_type(type), m_hash(genSymbolID(symName))
    {
    }

    const String& ISymbol::getName() const {
        return m_name;
    }

    const String& ISymbol::getSymbolName() const {
        return m_symName;
    }

    SymbolType ISymbol::getSymbolType() const {
        return m_type;
    }

    symbol_id ISymbol::getSymbolId() const {
        return m_hash;
    }

    String ISymbol::genNamespaceSymbolName(Namespace* ns, const String& name) {
        String ret;
        if (ns) ret = ns->getSymbolName();
        ret += ".N";
        ret += name;
        return ret;
    }

    String ISymbol::genFuncSymbolName(Namespace* ns, const String& name, FunctionType* sig) {
        String ret = sig->getReturnType()->getSymbolName() + "_";
        if (ns) ret += ns->getSymbolName();
        ret += ".F";
        ret += name;

        ret += "(";
        const Array<FunctionType::Argument>& args = sig->getArgs();
        for (u32 i = 0;i < args.size();i++) {
            if (i > 0) ret += ",";
            ret += args[i].type->getSymbolName();
        }
        ret += ")";

        return ret;
    }

    String ISymbol::genTypeSymbolName(Namespace* ns, const String& name) {
        String ret;
        if (ns) ret = ns->getSymbolName();
        ret += ".T";
        ret += name;
        return ret;
    }

    String ISymbol::genValueSymbolName(Namespace* ns, const String& name) {
        String ret;
        if (ns) ret = ns->getSymbolName();
        ret += ".V";
        ret += name;
        return ret;
    }

    symbol_id ISymbol::genSymbolID(const String& symName) {
        std::hash<std::string> h;
        return h(symName);
    }
};