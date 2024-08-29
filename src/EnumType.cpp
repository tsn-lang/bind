#include <bind/EnumType.h>
#include <bind/util/meta.hpp>
#include <utils/Array.hpp>
#include <utils/Exception.h>

namespace bind {
    type_meta enum_meta(u8 bits) {
        type_meta m;
        switch (bits) {
            case 8: {
                m = meta<u8>();
                m.is_enum = 1;
                return m;
            }
            case 16: {
                m = meta<u16>();
                m.is_enum = 1;
                return m;
            }
            case 32: {
                m = meta<u32>();
                m.is_enum = 1;
                return m;
            }
            case 64: {
                m = meta<u64>();
                m.is_enum = 1;
                return m;
            }
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