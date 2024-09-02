#pragma once
#include <bind/interfaces/ITypeBuilder.h>
#include <bind/interfaces/ICallHandler.h>
#include <bind/util/meta.hpp>
#include <bind/Registry.hpp>
#include <bind/EnumType.h>
#include <bind/Function.h>
#include <utils/Exception.h>
#include <utils/Pointer.hpp>

namespace bind {
    template <typename Prim>
    class EnumTypeBuilder : public ITypeBuilder {
        public:
            EnumTypeBuilder(const String& name, Namespace* ns)
                : ITypeBuilder(new EnumType(name, sizeof(Prim) * 8, ns)), m_hasDtor(false)
            {
                Registry::Add(m_type, type_hash<Prim>());
            }

            EnumTypeBuilder(EnumType* extend)
                : ITypeBuilder(extend), m_hasDtor(false) {
            }

            EnumType::Field& addEnumValue(const String& name, u64 value) {
                return _addEnumValue(name, value);
            }

            template <typename... Args>
            DataType::Property& ctor(void (*fn)(Prim*, Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_ctor = 1;

                Function* func = new Function(
                    ConstructorName,
                    fn,
                    Registry::Signature<void, Prim*, Args...>(),
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

            DataType::Property& dtor(void (*fn)(Prim*)) {
                if (m_hasDtor) {
                    throw Exception(String::Format("EnumTypeBuilder::dtor - Type '%s' already has a destructor", m_type->getFullName().c_str()));
                }

                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_dtor = 1;

                Function* func = new Function(
                    DestructorName,
                    fn,
                    Registry::Signature<void, Prim*>(),
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
            DataType::Property& method(const String& name, Ret (*fn)(Prim*, Args...)) {
                DataType::Property::Flags f = { 0 };
                f.can_read = 1;
                f.is_pseudo_method = 1;

                Function* func = new Function(
                    name,
                    fn,
                    Registry::Signature<Ret, Prim*, Args...>(),
                    m_type->getOwnNamespace()
                );

                func->setCallHandler(HostCallHandler::get());

                Registry::Add(func);

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

                Registry::Add(func);

                return addProperty(
                    Pointer(func),
                    f,
                    func->getSignature(),
                    name
                );
            }

            template <typename Ret>
            DataType::Property& castOperator(const String& name, Ret (*fn)(Prim*)) {
                return method(CastOperatorName, fn);
            }

            template <typename T>
            DataType::Property& staticProp(const String& name, T* member) {
                DataType* tp = Registry::GetType<T>();
                if (!tp) {
                    throw Exception(String::Format(
                        "EnumTypeBuilder::staticProp - Type '%s' for property '%s' of '%s' has not been registered",
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