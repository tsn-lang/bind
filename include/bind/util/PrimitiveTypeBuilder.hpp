#pragma once
#include <bind/interfaces/ITypeBuilder.h>
#include <bind/util/meta.hpp>
#include <bind/util/Exception.h>
#include <bind/Registry.hpp>
#include <bind/util/Pointer.hpp>

namespace bind {
    template <typename Prim>
    class PrimitiveTypeBuilder : public ITypeBuilder {
        public:
            PrimitiveTypeBuilder(const String& name, Namespace* ns)
                : ITypeBuilder(name, meta<Prim>(), ns, type_hash<Prim>()), m_hasDtor(false) { }
            PrimitiveTypeBuilder(DataType* extend) : ITypeBuilder(extend), m_hasDtor(false) { }

            template <typename... Args>
            DataType::Property& ctor(void (*fn)(Prim*, Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_ctor = 1;

                return addProperty(
                    Pointer(fn),
                    f,
                    Registry::Signature<void, Prim*, Args...>(),
                    "constructor"
                );
            }

            DataType::Property& dtor(void (*fn)(Prim*)) {
                if (m_hasDtor) {
                    throw Exception(String::Format("PrimitiveTypeBuilder::dtor - Type '%s' already has a destructor", m_type->getName().c_str()));
                }

                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_dtor = 1;

                return addProperty(
                    Pointer(fn),
                    f,
                    Registry::Signature<void, Prim*>(),
                    "destructor"
                );
            }

            template <typename Ret, typename... Args>
            DataType::Property& method(const String& name, Ret (*fn)(Prim*, Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_pseudo_method = 1;

                return addProperty(
                    Pointer(fn),
                    f,
                    Registry::Signature<Ret, Prim*, Args...>(),
                    name
                );
            }

            template <typename Ret, typename... Args>
            DataType::Property& staticMethod(const String& name, Ret (*fn)(Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_method = 1;
                f.is_static = 1;

                return addProperty(
                    Pointer(fn),
                    f,
                    Registry::Signature<Ret, Args...>(),
                    name
                );
            }

            template <typename T>
            DataType::Property& staticProp(const String& name, T* member) {
                DataType* tp = Registry::Get<T>();
                if (!tp) {
                    throw Exception(String::Format(
                        "PrimitiveTypeBuilder::staticProp - Type '%s' for property '%s' of '%s' has not been registered",
                        type_name<T>(),
                        name.c_str(),
                        m_type->getName().c_str()
                    ));
                }

                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.can_write = 1;
                f.is_static = 1;

                return addProperty((void*)member, f, tp, name);
            }
        
        protected:
            bool m_hasDtor;
    };
};