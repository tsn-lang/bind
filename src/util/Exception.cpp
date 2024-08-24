#include <bind/util/Exception.h>

namespace bind {
    Exception::Exception(const String& msg) : m_msg(msg) {}

    const String& Exception::message() const {
        return m_msg;
    }
};