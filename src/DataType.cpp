#include <bind/DataType.h>
#include <bind/AliasType.h>
#include <bind/FunctionType.h>
#include <bind/Namespace.h>
#include <utils/interfaces/IWithUserData.hpp>
#include <utils/Array.hpp>

namespace bind {
    DataType::Property::Property(
        i32 _offset,
        DataType::Property::Flags _flags,
        DataType* _type,
        const String& _name,
        AccessFlags _accessFlags
    ) : offset(_offset), flags(_flags), type(_type), name(_name), accessFlags(_accessFlags)
    {
        if (flags.is_ctor || flags.is_dtor || flags.is_method || flags.is_pseudo_method || flags.is_static) {
            if (offset != -1) {
                throw Exception("Only non-static object members can have offset >= 0");
            }
        } else {
            if (offset == -1) {
                throw Exception("Non-static object members must have offset >= 0");
            }
        }
    }

    DataType::Property::Property(
        const Pointer& _address,
        DataType::Property::Flags _flags,
        DataType* _type,
        const String& _name,
        AccessFlags _accessFlags
    ) : offset(-1), address(_address), flags(_flags), type(_type), name(_name), accessFlags(_accessFlags)
    {}
    
    DataType::Property::Property(const Property& prop)
        : offset(prop.offset), address(prop.address), flags(prop.flags),
          type(prop.type), name(prop.name), accessFlags(prop.accessFlags)
    {
    }

    DataType::DataType(const String& name, Namespace* ns)
        : ISymbol(name, ISymbol::genFullSymbolName(ns, name), ISymbol::genTypeSymbolName(ns, name), SymbolType::DataType),
          m_ownNamespace(new Namespace(ns, name)), m_pointerToSelf(nullptr)
    {
        memset(&m_info, 0, sizeof(type_meta));
    }

    DataType::DataType(const String& name, const type_meta& meta, Namespace* ns)
        : ISymbol(name, ISymbol::genFullSymbolName(ns, name), ISymbol::genTypeSymbolName(ns, name), SymbolType::DataType),
          m_info(meta), m_ownNamespace(new Namespace(ns, name)), m_pointerToSelf(nullptr)
    {
    }

    DataType::DataType(const String& name, const String& fullName, const type_meta& meta, Namespace* ns)
        : ISymbol(name, fullName, ISymbol::genTypeSymbolName(ns, name), SymbolType::DataType),
          m_info(meta), m_ownNamespace(new Namespace(ns, name)), m_pointerToSelf(nullptr)
    {
    }

    DataType::~DataType() {
        delete m_ownNamespace;
        m_ownNamespace = nullptr;
    }

    const type_meta& DataType::getInfo() const {
        return m_info;
    }
    
    Namespace* DataType::getOwnNamespace() const {
        return m_ownNamespace;
    }

    const Array<DataType::Property>& DataType::getProps() const {
        return m_props;
    }
    
    Array<const DataType::Property*> DataType::findProperties(const String& name, AccessFlags accessMask) const {
        Array<const Property*> ret;

        for (u32 i = 0;i < m_props.size();i++) {
            const Property& p = m_props[i];
            if (p.accessFlags != PublicAccess && (p.accessFlags & accessMask) != p.accessFlags) continue;
            if (p.name == name) ret.push(&p);
        }

        return ret;
    }
    
    Array<Function*> DataType::findMethods(const FuncMatch& search, Function** singleStrictMatch) const {
        if (singleStrictMatch) *singleStrictMatch = nullptr;

        Array<Function*> ret;

        u32 strictMatchCount = 0;

        for (u32 i = 0;i < m_props.size();i++) {
            const Property& p = m_props[i];
            if (p.flags.is_method == 0 && p.flags.is_pseudo_method == 0) continue;
            if (p.accessFlags != PublicAccess && (p.accessFlags & search.accessMask) != p.accessFlags) continue;

            FunctionType* sig = (FunctionType*)p.type;
            
            if (p.name.size() != search.name.size()) continue;
            if (p.name != search.name) continue;

            bool isStrictMatch = true;
            
            if (search.returnType) {
                if (search.returnTypeStrict) {
                    if (!sig->getReturnType()->isEquivalentTo(search.returnType)) continue;
                } else {
                    if (!sig->getReturnType()->isConvertibleTo(search.returnType)) continue;

                    if (strictMatchCount <= 1 && singleStrictMatch && !sig->getReturnType()->isEquivalentTo(search.returnType)) {
                        isStrictMatch = false;
                    }
                }
            }

            if (search.doCheckArgs) {
                auto& args = sig->getArgs();

                // offset arg count by 1 for pseudo methods because they are forced by the bind API
                // to accept the 'this' parameter as the first argument, but we don't want to force
                // that to have to be included in function searches
                if (!p.flags.is_pseudo_method && search.argTypes.size() != args.size()) continue;
                else if (p.flags.is_pseudo_method && search.argTypes.size() != args.size() - 1) continue;

                bool isMatch = true;
                for (u32 a = p.flags.is_pseudo_method ? 1 : 0;a < args.size();a++) {
                    DataType* providedType;
                    if (p.flags.is_pseudo_method) providedType = search.argTypes[a - 1];
                    else providedType = search.argTypes[a];

                    if (search.argTypesStrict) {
                        if (!args[a].type->isEquivalentTo(providedType)) {
                            isMatch = false;
                            break;
                        }
                    } else {
                        if (!args[a].type->isConvertibleTo(providedType)) {
                            isMatch = false;
                            break;
                        }

                        if (strictMatchCount <= 1 && isStrictMatch && singleStrictMatch && !args[a].type->isEquivalentTo(providedType)) {
                            isStrictMatch = false;
                        }
                    }
                }

                if (!isMatch) continue;
            }

            ret.push((Function*)p.address.get());

            if (isStrictMatch && singleStrictMatch) {
                if (strictMatchCount == 0) *singleStrictMatch = (Function*)p.address.get();
                else *singleStrictMatch = nullptr;

                strictMatchCount++;
            }
        }

        return ret;
    }

    Array<Function*> DataType::findConstructors(
        const Array<DataType*>& argTypes,
        bool strict,
        AccessFlags accessMask,
        Function** singleStrictMatch
    ) const {
        if (singleStrictMatch) *singleStrictMatch = nullptr;

        Array<Function*> ret;

        u32 strictMatchCount = 0;

        for (u32 i = 0;i < m_props.size();i++) {
            const Property& p = m_props[i];
            if (p.flags.is_ctor == 0) continue;
            if (p.accessFlags != PublicAccess && (p.accessFlags & accessMask) != p.accessFlags) continue;

            bool isStrictMatch = true;

            FunctionType* sig = (FunctionType*)p.type;
            
            if (p.name.size() != ConstructorNameLen) continue;
            if (p.name != ConstructorName) continue;
            
            auto& args = sig->getArgs();
            // offset this and the loop by 1 because constructors are wrapped and the wrapper
            // takes the memory to construct the object in as the first parameter, then the
            // actual parameters come after that
            if (argTypes.size() != args.size() - 1) continue;

            bool isMatch = true;
            for (u32 a = 1;a < args.size();a++) {
                if (strict) {
                    if (!args[a].type->isEquivalentTo(argTypes[a - 1])) {
                        isMatch = false;
                        break;
                    }
                } else {
                    if (!args[a].type->isConvertibleTo(argTypes[a - 1])) {
                        isMatch = false;
                        break;
                    }

                    if (strictMatchCount <= 1 && isStrictMatch && singleStrictMatch && !args[a].type->isEquivalentTo(argTypes[a])) {
                        isStrictMatch = false;
                    }
                }
            }

            if (!isMatch) continue;

            ret.push((Function*)p.address.get());

            if (isStrictMatch && singleStrictMatch) {
                if (strictMatchCount == 0) *singleStrictMatch = (Function*)p.address.get();
                else *singleStrictMatch = nullptr;

                strictMatchCount++;
            }
        }

        return ret;
    }

    Function* DataType::findConversionOperator(DataType* resultType, AccessFlags accessMask) const {
        for (u32 i = 0;i < m_props.size();i++) {
            const Property& p = m_props[i];
            if (p.flags.is_method == 0 && p.flags.is_pseudo_method == 0) continue;
            if (p.accessFlags != PublicAccess && (p.accessFlags & accessMask) != p.accessFlags) continue;

            FunctionType* sig = (FunctionType*)p.type;
            
            if (sig->getArgs().size() != 0) continue;
            if (p.name.size() != CastOperatorNameLen) continue;
            if (p.name != CastOperatorName) continue;

            if (!sig->getReturnType()->isEquivalentTo(resultType)) continue;

            return (Function*)p.address.get();
        }

        return nullptr;
    }

    bool DataType::isConvertibleTo(DataType* _to, AccessFlags accessMask) const {
        if (!_to) return false;
        DataType* to = _to->getEffectiveType();

        // all primitives are convertible to each other
        if (m_info.is_primitive && to->m_info.is_primitive) return true;
        if (m_info.is_pointer && to->m_info.is_pointer) return true;

        // no conversion necessary
        if (isEqualTo(to)) return true;

        auto castMethods = findMethods(
            FuncMatch(CastOperatorName)
            .retTp(to, true)
            .noArgs()
            .access(accessMask)
        );

        // conversion via cast operator
        if (castMethods.size() == 1) return true;

        DataType* self = getEffectiveType();

        auto copyCtors = to->findMethods(
            FuncMatch(ConstructorName)
            .argTps({ self }, true)
            .access(accessMask)
        );

        // conversion via copy constructor
        if (copyCtors.size() == 1) return true;

        //
        // See if this type can be shoehorned into the other type
        //
        if (!to->m_info.is_trivially_copyable) {
            // Nope, other type requires a constructor to be called
            return false;
        }

        for (u32 i = 0;i < to->m_props.size();i++) {
            auto& rProp = to->m_props[i];
            if (rProp.offset == -1) continue;

            bool found = false;
            for (u32 j = 0;j < m_props.size();j++) {
                auto& sProp = m_props[j];
                if (sProp.offset == -1) continue;
                if (sProp.name != rProp.name) continue;
                if (!sProp.type->isConvertibleTo(rProp.type)) {
                    // Nope, this property of the other type cannot be
                    // converted to from our property with the same name
                    return false;
                }

                found = true;
                break;
            }

            if (!found) {
                // Nope, this property of the other type has no counterpart
                // on this type
                return false;
            }
        }

        // Yup
        return true;
    }

    bool DataType::isImplicitlyAssignableTo(DataType* to) const {
        if (!to) return false;
        if (m_info.is_primitive && to->m_info.is_primitive) return true;

        return isEquivalentTo(to) && m_info.is_trivially_copyable && to->m_info.is_trivially_copyable;
    }

    bool DataType::isEquivalentTo(DataType* _to) const {
        if (!_to) return false;

        DataType* to = _to->getEffectiveType();
        if (isEqualTo(to)) return true;

        if (
            m_info.size                         != to->m_info.size                          ||
            m_info.is_pod                       != to->m_info.is_pod                        ||
            m_info.is_trivially_constructible   != to->m_info.is_trivially_constructible    ||
            m_info.is_trivially_copyable        != to->m_info.is_trivially_copyable         ||
            m_info.is_trivially_destructible    != to->m_info.is_trivially_destructible     ||
            m_info.is_primitive                 != to->m_info.is_primitive                  ||
            m_info.is_floating_point            != to->m_info.is_floating_point             ||
            m_info.is_integral                  != to->m_info.is_integral                   ||
            m_info.is_unsigned                  != to->m_info.is_unsigned                   ||
            m_info.is_function                  != to->m_info.is_function                   ||
            m_props.size()                      != to->m_props.size()
        ) return false;

        const DataType* self = getEffectiveType();

        for (u32 i = 0;i < self->m_props.size();i++) {
            const Property& p1 = self->m_props[i];
            const Property& p2 = to->m_props[i];

            if (p1.offset != p2.offset) return false;
            if (p1.accessFlags != p2.accessFlags) return false;
            if (p1.flags.can_read != p2.flags.can_read) return false;
            if (p1.flags.can_write != p2.flags.can_write) return false;
            if (p1.flags.is_static != p2.flags.is_static) return false;
            if (p1.flags.is_method != p2.flags.is_method) return false;
            if (p1.flags.is_pseudo_method != p2.flags.is_pseudo_method) return false;
            if (p1.flags.is_ctor != p2.flags.is_ctor) return false;
            if (p1.flags.is_dtor != p2.flags.is_dtor) return false;
            if (p1.name.size() != p2.name.size() || p1.name != p2.name) return false;
            if (!p1.type->isEqualTo(p2.type)) return false;
        }

        return true;
    }

    bool DataType::isEqualTo(DataType* to) const {
        if (!to) return false;
        return getEffectiveType()->getSymbolId() == to->getEffectiveType()->getSymbolId();
    }

    bool DataType::isConstructableWith(const Array<DataType*>& args, AccessFlags accessMask, bool strict) const {
        auto ctors = getEffectiveType()->findMethods(
            FuncMatch(ConstructorName)
            .argTps(args, strict)
            .access(accessMask)
        );

        return ctors.size() == 1;
    }

    DataType* DataType::getEffectiveType() const {
        if (m_info.is_alias == 0) return const_cast<DataType*>(this);
        return ((AliasType*)this)->getBaseType();
    }

    PointerType* DataType::getPointerType() {
        if (m_pointerToSelf) return m_pointerToSelf;

        m_pointerToSelf = new PointerType(this);
        Registry::Add(m_pointerToSelf);
        return m_pointerToSelf;
    }
};