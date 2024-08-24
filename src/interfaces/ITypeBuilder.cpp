#include <bind/interfaces/ITypeBuilder.h>
#include <bind/interfaces/IWithUserData.hpp>
#include <bind/util/Exception.h>
#include <bind/DataType.h>
#include <bind/Registry.h>
#include <bind/util/Array.hpp>

namespace bind {
    ITypeBuilder::ITypeBuilder(const String& name, const type_meta& meta, Namespace* ns, size_t nativeHash) {
        m_type = new DataType(name, meta, ns);
        Registry::Add(m_type, nativeHash);
    }

    ITypeBuilder::ITypeBuilder(DataType* extend) {
        m_type = extend;
    }

    ITypeBuilder::~ITypeBuilder() {
        m_type = nullptr;
    }

    DataType::Property& ITypeBuilder::addProperty(
        i32 offset,
        DataType::Property::Flags flags,
        DataType* type,
        String name
    ) {
        m_type->m_props.push(DataType::Property(offset, flags, type, name));
        return m_type->m_props.last();
    }

    DataType::Property& ITypeBuilder::addProperty(
        const Pointer& address,
        DataType::Property::Flags flags,
        DataType* type,
        String name
    ) {
        m_type->m_props.push(DataType::Property(address, flags, type, name));
        return m_type->m_props.last();
    }
    
    EnumType::Field& ITypeBuilder::_addEnumValue(const String& name, u64 value) {
        ((EnumType*)m_type)->m_fields.push({ name, value });
        ((EnumType*)m_type)->m_fieldIndexMap.insert(std::pair<String, u32>(name, ((EnumType*)m_type)->m_fields.size() - 1));
        return ((EnumType*)m_type)->m_fields.last();
    }

    ITypeBuilder::operator DataType*() const {
        return m_type;
    }

    DataType* ITypeBuilder::getType() const {
        return m_type;
    }
};