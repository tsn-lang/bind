#include <bind/PointerType.h>
#include <bind/util/Array.hpp>
#include <bind/util/meta.hpp>
#include <string.h>

namespace bind {
    PointerType::PointerType(DataType* pointsTo)
        : DataType(pointsTo->getName() + "*", meta<void*>(), nullptr), m_destTp(pointsTo), m_baseType(pointsTo)
    {
        if (m_destTp->getInfo().is_pointer) {
            m_baseType = ((PointerType*)m_destTp)->m_baseType;
        }
    }

    PointerType::~PointerType() {
    }

    DataType* PointerType::getDestinationType() const {
        return m_destTp;
    }

    DataType* PointerType::getBaseType() const {
        return m_baseType;
    }
};