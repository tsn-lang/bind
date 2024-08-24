#include <bind/FunctionType.h>
#include <bind/util/Array.hpp>
#include <string.h>

namespace bind {
    FunctionType::Argument::Argument(u8 _index, DataType* _type)
        : index(_index), type(_type)
    {
    }

    FunctionType::FunctionType(const String& name, const type_meta& meta)
        : DataType(name, meta, nullptr), m_returnType(nullptr), m_thisType(nullptr)
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