#pragma once
#include <bind/DataType.h>

namespace bind {
    class FunctionType : public DataType {
        public:
            struct Argument : public IWithFixedUserData<32> {
                Argument(u8 index, DataType* type);
                
                u8 index;
                DataType* type;
            };

            FunctionType(const String& name, const type_meta& meta);
            ~FunctionType();

            const Array<Argument>& getArgs() const;
            DataType* getReturnType() const;
            DataType* getThisType() const;
        
        protected:
            friend class Registry;

            Array<Argument> m_args;
            DataType* m_returnType;
            DataType* m_thisType;
    };
};