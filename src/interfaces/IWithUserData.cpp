#include <bind/interfaces/IWithUserData.h>

namespace bind {
    IWithUserData::IWithUserData() : m_userData(nullptr) {
    }

    IWithUserData::~IWithUserData() {
        m_userData = nullptr;
    }
};