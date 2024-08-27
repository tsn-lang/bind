#pragma once
#include <bind/types.h>
#include <utils/String.h>

namespace bind {
    class Namespace;
    class FunctionType;

    enum class SymbolType {
        Namespace,
        Function,
        DataType,
        Value
    };

    class ISymbol {
        public:
            ISymbol(const String& name, const String& symName, SymbolType type);

            const String& getName() const;
            const String& getSymbolName() const;
            SymbolType getSymbolType() const;
            symbol_id getSymbolId() const;

            static String genNamespaceSymbolName(Namespace* ns, const String& name);
            static String genFuncSymbolName(Namespace* ns, const String& name, FunctionType* sig);
            static String genTypeSymbolName(Namespace* ns, const String& name);
            static String genValueSymbolName(Namespace* ns, const String& name);
            static symbol_id genSymbolID(const String& symName);
        
        private:
            String m_name;
            String m_symName;
            SymbolType m_type;
            symbol_id m_hash;
    };
};