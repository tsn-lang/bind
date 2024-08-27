#include <bind/interfaces/ISymbol.h>

namespace bind {
    class DataType;

    class Value : public ISymbol {
        public:
            Value(const String& name, DataType* type, void* address, Namespace* ns);
            ~Value();

            DataType* getType() const;
            void* getAddress() const;

        protected:
            DataType* m_type;
            void* m_address;
    };
};