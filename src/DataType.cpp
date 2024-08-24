#include <bind/DataType.h>
#include <bind/interfaces/IWithUserData.hpp>
#include <bind/util/Array.hpp>
#include <string.h>

namespace bind {
    DataType::Property::Property(
        i32 _offset,
        DataType::Property::Flags _flags,
        DataType* _type,
        const String& _name
    ) : offset(_offset), flags(_flags), type(_type), name(_name) {
    }

    DataType::Property::Property(
        const Pointer& _address,
        DataType::Property::Flags _flags,
        DataType* _type,
        const String& _name
    ) : offset(-1), address(_address), flags(_flags), type(_type), name(_name) {
    }
    
    DataType::Property::Property(const Property& prop)
        : offset(prop.offset), address(prop.address), flags(prop.flags),
          type(prop.type), name(prop.name)
    {
    }

    DataType::DataType(const String& name, Namespace* ns)
        : ISymbol(name, ISymbol::genTypeSymbolName(ns, name), SymbolType::DataType)
    {
        memset(&m_info, 0, sizeof(type_meta));
    }

    DataType::DataType(const String& name, const type_meta& meta, Namespace* ns)
        : ISymbol(name, ISymbol::genTypeSymbolName(ns, name), SymbolType::DataType),
          m_info(meta)
    {
    }

    DataType::~DataType() {
    }

    const type_meta& DataType::getInfo() const {
        return m_info;
    }

    const Array<DataType::Property>& DataType::getProps() const {
        return m_props;
    }
    
    Array<const DataType::Property*> DataType::findProperties(const String& name) const {
        Array<const Property*> ret;

        for (u32 i = 0;i < m_props.size();i++) {
            if (m_props[i].name == name) ret.push(&m_props[i]);
        }

        return ret;
    }
};