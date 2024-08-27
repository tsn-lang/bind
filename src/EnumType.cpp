#include <bind/EnumType.h>
#include <bind/util/meta.hpp>
#include <utils/Array.hpp>
#include <utils/Exception.h>

namespace bind {
    type_meta enum_meta(u8 bits) {
        switch (bits) {
            case 8: return meta<u8>();
            case 16: return meta<u16>();
            case 32: return meta<u32>();
            case 64: return meta<u64>();
        }

        throw Exception("EnumType - Invalid size. Should be 8, 16, 32, or 64");
    }

    EnumType::EnumType(const String& name, u8 bits, Namespace* ns)
        : DataType(name, enum_meta(bits), ns)
    {
    }

    EnumType::~EnumType() {
    }

    const Array<EnumType::Field>& EnumType::getFields() const {
        return m_fields;
    }
    
    const EnumType::Field* EnumType::getField(const String& name) const {
        auto it = m_fieldIndexMap.find(name);
        if (it == m_fieldIndexMap.end()) return nullptr;

        return &m_fields[it->second];
    }
};