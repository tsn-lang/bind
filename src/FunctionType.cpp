#include <bind/FunctionType.h>
#include <utils/Array.hpp>

namespace bind {
    type_meta func_meta(const type_meta& m) {
        type_meta ret = m;
        ret.is_function = 1;
        return ret;
    }

    FunctionType::Argument::Argument(u8 _index, DataType* _type)
        : index(_index), type(_type)
    {
    }

    FunctionType::FunctionType(const String& name, const type_meta& meta)
        : DataType(name, func_meta(meta), nullptr), m_returnType(nullptr), m_thisType(nullptr)
    {
    }

    FunctionType::~FunctionType() {
    }

    const Array<FunctionType::Argument>& FunctionType::getArgs() const {
        return m_args;
    }

    DataType* FunctionType::getReturnType() const {
        return m_returnType;
    }

    DataType* FunctionType::getThisType() const {
        return m_thisType;
    }
};