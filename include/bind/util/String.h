#pragma once
#include <bind/types.h>
#include <string>
#include <unordered_map>

namespace bind {
    class String {
        public:
            String();
            String(const char* cstr);
            String(const std::string& str);
            String(const String& str);
            ~String();

            u32 size() const;
            const char* c_str() const;
            size_t hash() const;

            String& operator =(const String& rhs);
            String& operator =(const char* rhs);
            String& operator =(const std::string& rhs);
            String& operator +=(const String& rhs);
            String& operator +=(const char* rhs);
            String& operator +=(const std::string& rhs);
            String operator +(const String& rhs) const;
            String operator +(const char* rhs) const;
            String operator +(const std::string& rhs) const;
            bool operator ==(const String& rhs) const;
            bool operator ==(const char* rhs) const;
            bool operator ==(const std::string& rhs) const;

            operator std::string() const;
            
            static String Format(const char* fmt, ...);

        protected:
            void copy(const char* src, u32 len);
            void append(const char* src, u32 len);
            void extend(u32 newCapacity);

            char* m_str;
            u32 m_len;
            u32 m_capacity;
    };
};

template <>
struct std::hash<bind::String> {
    std::size_t operator()(const bind::String& k) const;
};