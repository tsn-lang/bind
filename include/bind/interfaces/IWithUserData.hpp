#pragma once
#include <bind/interfaces/IWithUserData.h>
#include <string.h>

namespace bind {
    template <size_t maxDataSize>
    IWithFixedUserData<maxDataSize>::IWithFixedUserData() {
        memset(m_userData, 0, maxDataSize);
    }

    template <size_t maxDataSize>
    IWithFixedUserData<maxDataSize>::~IWithFixedUserData() {
    }

    template <size_t maxDataSize>
    template <typename UserDataType>
    void IWithFixedUserData<maxDataSize>::setUserData(const UserDataType& data) {
        static_assert(
            sizeof(UserDataType) <= maxDataSize,
            "User defined data type exceeds allowed size"
        );

        memcpy(m_userData, &data, sizeof(UserDataType));
    }

    template <size_t maxDataSize>
    template <typename UserDataType>
    UserDataType& IWithFixedUserData<maxDataSize>::getUserData() {
        static_assert(
            sizeof(UserDataType) <= maxDataSize,
            "User defined data type exceeds allowed size"
        );

        return *(UserDataType*)m_userData;
    }

    template <typename UserDataType>
    void IWithUserData::setUserData(UserDataType* data) {
        m_userData = data;
    }

    template <typename UserDataType>
    UserDataType* IWithUserData::getUserData() {
        return (UserDataType*)m_userData;
    }
};