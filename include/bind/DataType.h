#pragma once
#include <bind/types.h>
#include <bind/interfaces/ISymbol.h>
#include <utils/interfaces/IWithUserData.h>
#include <utils/String.h>
#include <utils/Array.h>
#include <utils/Pointer.h>

namespace bind {
    class DataType : public IWithFixedUserData<32>, public ISymbol {
        public:
            struct Property : public IWithFixedUserData<32> {
                struct Flags {
                    unsigned can_read : 1;
                    unsigned can_write : 1;
                    unsigned is_static : 1;
                    unsigned is_method : 1;
                    unsigned is_pseudo_method : 1;
                    unsigned is_ctor : 1;
                    unsigned is_dtor : 1;
                    unsigned __pad0 : 1;
                };

                Property(i32 offset, Flags flags, DataType* type, const String& name);
                Property(const Pointer& address, Flags flags, DataType* type, const String& name);
                Property(const Property& prop);
                
                i32 offset;
                Pointer address;
                Flags flags;
                DataType* type;
                String name;
            };

            DataType(const String& name, Namespace* ns);
            DataType(const String& name, const type_meta& meta, Namespace* ns);
            ~DataType();

            const type_meta& getInfo() const;
            const Array<Property>& getProps() const;
            Array<const Property*> findProperties(const String& name) const;

        protected:
            friend class ITypeBuilder;

            type_meta m_info;
            Array<Property> m_props;
    };
};