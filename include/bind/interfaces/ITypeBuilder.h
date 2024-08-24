#pragma once
#include <bind/types.h>
#include <bind/util/String.h>
#include <bind/DataType.h>
#include <bind/EnumType.h>

namespace bind {
    class Namespace;
    
    class ITypeBuilder {
        public:
            ITypeBuilder(const String& name, const type_meta& meta, Namespace* ns, size_t nativeHash);
            ITypeBuilder(DataType* extend);
            virtual ~ITypeBuilder();

            DataType::Property& addProperty(
                i32 offset,
                DataType::Property::Flags flags,
                DataType* type,
                String name
            );

            DataType::Property& addProperty(
                const Pointer& address,
                DataType::Property::Flags flags,
                DataType* type,
                String name
            );

            operator DataType*() const;
            DataType* getType() const;

        protected:
            EnumType::Field& _addEnumValue(const String& name, u64 value);

            DataType* m_type;
    };
};