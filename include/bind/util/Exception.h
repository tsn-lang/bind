#pragma once
#include <bind/util/String.h>

namespace bind {
    class Exception {
        public:
            Exception(const String& msg);
        
            const String& message() const;
            
        protected:
            String m_msg;
    };
};