#include <bind/DataType.h>
#include <bind/AliasType.h>
#include <bind/FunctionType.h>
#include <bind/Namespace.h>
#include <bind/interfaces/IWithUserData.hpp>
#include <bind/util/Array.hpp>
#include <string.h>

namespace bind {
    DataType::Property::Property(
        i32 _offset,
        DataType::Property::Flags _flags,
        DataType* _type,
        const String& _name,
        AccessFlags _accessFlags
    ) : offset(_offset), flags(_flags), type(_type), name(_name), accessFlags(_accessFlags)
    {}

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
        : ISymbol(name, ISymbol::genTypeSymbolName(ns, name), SymbolType::DataType),
          m_ownNamespace(new Namespace(ns, name))
    {
        memset(&m_info, 0, sizeof(type_meta));
    }

    DataType::DataType(const String& name, const type_meta& meta, Namespace* ns)
        : ISymbol(name, ISymbol::genTypeSymbolName(ns, name), SymbolType::DataType),
          m_info(meta), m_ownNamespace(new Namespace(ns, name))
    {
    }

    DataType::~DataType() {
        delete m_ownNamespace;
        m_ownNamespace = nullptr;
    }

    const type_meta& DataType::getInfo() const {
        return m_info;
    }
    
    const Namespace* DataType::getOwnNamespace() const {
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
                if (search.argTypes.size() != args.size()) continue;

                bool isMatch = true;
                for (u32 a = 0;a < args.size();a++) {
                    if (search.argTypesStrict) {
                        if (!args[a].type->isEquivalentTo(search.argTypes[a])) {
                            isMatch = false;
                            break;
                        }
                    } else {
                        if (!args[a].type->isConvertibleTo(search.argTypes[a])) {
                            isMatch = false;
                            break;
                        }

                        if (strictMatchCount <= 1 && isStrictMatch && singleStrictMatch && !args[a].type->isEquivalentTo(search.argTypes[a])) {
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
            if (argTypes.size() != args.size()) continue;

            bool isMatch = true;
            for (u32 a = 0;a < args.size();a++) {
                if (strict) {
                    if (!args[a].type->isEquivalentTo(argTypes[a])) {
                        isMatch = false;
                        break;
                    }
                } else {
                    if (!args[a].type->isConvertibleTo(argTypes[a])) {
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

    bool DataType::isConvertibleTo(DataType* to, AccessFlags accessMask) const {
        if (!to) return false;
        if (m_info.is_primitive && to->m_info.is_primitive) return true;
        if (isEqualTo(to)) return true;

        DataType* toEffective = to;
        if (toEffective->m_info.is_alias) {
            toEffective = ((AliasType*)toEffective)->getBaseType();
        }
        
        auto castMethods = findMethods(
            FuncMatch(CastOperatorName)
            .retTp(toEffective, true)
            .noArgs()
            .access(accessMask)
        );

        if (castMethods.size() == 1) return true;

        DataType* self = to;
        if (m_info.is_alias) self = ((AliasType*)self)->getBaseType();

        auto copyCtors = to->findMethods(
            FuncMatch(ConstructorName)
            .argTps({ self }, true)
            .access(accessMask)
        );

        return copyCtors.size() == 1;
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
        return getEffectiveType()->getSymbolHash() == to->getEffectiveType()->getSymbolHash();
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
};