#pragma once
#include <bind/types.h>

namespace bind {
    template <size_t maxDataSize = 32>
    class IWithFixedUserData {
        public:
            IWithFixedUserData();
            virtual ~IWithFixedUserData();

            template <typename UserDataType>
            void setUserData(const UserDataType& T);

            template <typename UserDataType>
            UserDataType& getUserData();
        
        private:
            u8 m_userData[maxDataSize];
    };

    class IWithUserData {
        public:
            IWithUserData();
            virtual ~IWithUserData();

            template <typename UserDataType>
            void setUserData(UserDataType* T);

            template <typename UserDataType>
            UserDataType* getUserData();
        
        private:
            void* m_userData;
    };
};