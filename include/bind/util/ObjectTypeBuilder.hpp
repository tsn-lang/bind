#pragma once
#include <bind/interfaces/ITypeBuilder.h>
#include <bind/util/meta.hpp>
#include <bind/util/FuncWrap.hpp>
#include <bind/Registry.hpp>
#include <utils/Exception.h>
#include <utils/Pointer.hpp>

namespace bind {
    template <typename Cls>
    class ObjectTypeBuilder : public ITypeBuilder {
        public:
            ObjectTypeBuilder(const String& name, Namespace* ns)
                : ITypeBuilder(name, meta<Cls>(), ns, type_hash<Cls>()), m_hasDtor(false) { }
            ObjectTypeBuilder(DataType* extend) : ITypeBuilder(extend), m_hasDtor(false) { }

            template <typename... Args>
            DataType::Property& ctor() {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_ctor = 1;

                return addProperty(
                    Pointer(&_constructor_wrapper<Cls, Args...>),
                    f,
                    Registry::MethodSignature<void, Cls, Args...>(),
                    "constructor"
                );
            }

            DataType::Property& dtor() {
                if (m_hasDtor) {
                    throw Exception(String::Format("ObjectTypeBuilder::dtor - Type '%s' already has a destructor", m_type->getName().c_str()));
                }

                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_dtor = 1;

                return addProperty(
                    Pointer(&_destructor_wrapper<Cls>),
                    f,
                    Registry::MethodSignature<void, Cls>(),
                    "destructor"
                );
            }

            template <typename Ret, typename... Args>
            DataType::Property& method(const String& name, Ret (Cls::*fn)(Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_method = 1;

                return addProperty(
                    Pointer(fn),
                    f,
                    Registry::MethodSignature<Ret, Cls, Args...>(),
                    name
                );
            }

            template <typename Ret, typename... Args>
            DataType::Property& pseudoMethod(const String& name, Ret (*fn)(Cls*, Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_pseudo_method = 1;

                return addProperty(
                    Pointer(fn),
                    f,
                    Registry::Signature<Ret, Cls*, Args...>(),
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
            DataType::Property& prop(const String& name, T Cls::*member) {
                DataType* tp = Registry::Get<T>();
                if (!tp) {
                    throw Exception(String::Format(
                        "ObjectTypeBuilder::prop - Type '%s' for property '%s' of '%s' has not been registered",
                        type_name<T>(),
                        name.c_str(),
                        m_type->getName().c_str()
                    ));
                }

                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.can_write = 1;

                i32 offset = i32((u8*)&((Cls*)nullptr->*member) - (u8*)nullptr);
                return addProperty(offset, f, tp, name);
            }

            template <typename T>
            DataType::Property& staticProp(const String& name, T* member) {
                DataType* tp = Registry::Get<T>();
                if (!tp) {
                    throw Exception(String::Format(
                        "ObjectTypeBuilder::staticProp - Type '%s' for property '%s' of '%s' has not been registered",
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