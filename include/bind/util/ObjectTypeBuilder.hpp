#pragma once
#include <bind/interfaces/ITypeBuilder.h>
#include <bind/interfaces/ICallHandler.h>
#include <bind/util/meta.hpp>
#include <bind/util/FuncWrap.hpp>
#include <bind/Registry.hpp>
#include <bind/Function.h>
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

                Function* func = new Function(
                    ConstructorName,
                    &_constructor_wrapper<Cls, Args...>,
                    Registry::Signature<void, Cls*, Args...>(),
                    m_type->getOwnNamespace()
                );

                func->setCallHandler(HostCallHandler::get());

                Registry::Add(func);

                return addProperty(
                    Pointer(func),
                    f,
                    func->getSignature(),
                    ConstructorName
                );
            }

            DataType::Property& dtor() {
                if (m_hasDtor) {
                    throw Exception(String::Format("ObjectTypeBuilder::dtor - Type '%s' already has a destructor", m_type->getFullName().c_str()));
                }

                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_dtor = 1;

                Function* func = new Function(
                    DestructorName,
                    &_destructor_wrapper<Cls>,
                    Registry::Signature<void, Cls*>(),
                    m_type->getOwnNamespace()
                );

                func->setCallHandler(HostCallHandler::get());

                Registry::Add(func);

                return addProperty(
                    Pointer(func),
                    f,
                    func->getSignature(),
                    DestructorName
                );
            }

            template <typename Ret, typename... Args>
            DataType::Property& method(const String& name, Ret (Cls::*fn)(Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_method = 1;

                Function* func = new Function(
                    name,
                    fn,
                    Registry::MethodSignature<Ret, Cls, Args...>(),
                    m_type->getOwnNamespace()
                );

                func->setCallHandler(HostThisCallHandler::get());

                return addProperty(
                    Pointer(func),
                    f,
                    func->getSignature(),
                    name
                );
            }

            template <typename Ret, typename... Args>
            DataType::Property& pseudoMethod(const String& name, Ret (*fn)(Cls*, Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_pseudo_method = 1;

                Function* func = new Function(
                    name,
                    fn,
                    Registry::Signature<Ret, Cls*, Args...>(),
                    m_type->getOwnNamespace()
                );

                func->setCallHandler(HostCallHandler::get());

                return addProperty(
                    Pointer(func),
                    f,
                    func->getSignature(),
                    name
                );
            }

            template <typename Ret, typename... Args>
            DataType::Property& staticMethod(const String& name, Ret (*fn)(Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_method = 1;
                f.is_static = 1;

                Function* func = new Function(
                    name,
                    fn,
                    Registry::Signature<Ret, Args...>(),
                    m_type->getOwnNamespace()
                );

                func->setCallHandler(HostCallHandler::get());

                return addProperty(
                    Pointer(func),
                    f,
                    func->getSignature(),
                    name
                );
            }

            template <typename T>
            DataType::Property& prop(const String& name, T Cls::*member) {
                DataType* tp = Registry::GetType<T>();
                if (!tp) {
                    throw Exception(String::Format(
                        "ObjectTypeBuilder::prop - Type '%s' for property '%s' of '%s' has not been registered",
                        type_name<T>(),
                        name.c_str(),
                        m_type->getFullName().c_str()
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
                DataType* tp = Registry::GetType<T>();
                if (!tp) {
                    throw Exception(String::Format(
                        "ObjectTypeBuilder::staticProp - Type '%s' for property '%s' of '%s' has not been registered",
                        type_name<T>(),
                        name.c_str(),
                        m_type->getFullName().c_str()
                    ));
                }

                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.can_write = 1;
                f.is_static = 1;

                Registry::Add(new ValuePointer(name, tp, member, m_type->getOwnNamespace()));
                return addProperty((void*)member, f, tp, name);
            }
        
        protected:
            bool m_hasDtor;
    };
};