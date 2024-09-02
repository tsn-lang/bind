#pragma once
#include <bind/interfaces/ISymbol.h>
#include <bind/util/ObjectTypeBuilder.hpp>
#include <bind/util/PrimitiveTypeBuilder.hpp>
#include <bind/util/EnumTypeBuilder.hpp>
#include <utils/String.h>
#include <utils/interfaces/IWithUserData.h>
#include <unordered_map>

namespace bind {
    class AliasType;

    class Namespace : public IWithFixedUserData<32>, public ISymbol {
        public:
            Namespace(const String& name);
            Namespace(Namespace* parent, const String& name);
            Namespace(DataType* type);
            Namespace(Namespace* parent, DataType* type);

            template <typename T>
            std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
            build(const String& name);

            template <typename T>
            std::enable_if_t<std::is_class_v<T>, ObjectTypeBuilder<T>>
            extend();

            template <typename T>
            std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
            build(const String& name);

            template <typename T>
            std::enable_if_t<std::is_fundamental_v<T>, PrimitiveTypeBuilder<T>>
            extend();

            template <typename T>
            std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
            build(const String& name);

            template <typename T>
            std::enable_if_t<std::is_enum_v<T>, EnumTypeBuilder<T>>
            extend();

            template <typename T>
            ValuePointer* value(const String& name, T* val);

            template <typename Ret, typename... Args>
            Function* function(const String& name, Ret (*fn)(Args...));

            template <typename T>
            AliasType* alias(const String& name);

            AliasType* alias(const String& name, DataType* aliasOf);

            const String& getName() const;
            Namespace* getParent() const;
            DataType* getCorrespondingType() const;
            ISymbol* findSymbol(const String& symbolName) const;
        
        protected:
            String m_name;
            Namespace* m_parent;
            DataType* m_forType;
            std::unordered_map<u64, ISymbol*> m_symbolMap;
    };
};