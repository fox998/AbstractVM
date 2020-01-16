#include "Operand.hpp"

#define DEFINE_GET_TYPE(TYPE) \
template<> \
eOperandType Operand<TYPE>::getType() const \
{ \
    return eOperandType::TYPE; \
};

DEFINE_GET_TYPE(Int8);
DEFINE_GET_TYPE(Int16);
DEFINE_GET_TYPE(Int32);
DEFINE_GET_TYPE(Float);
DEFINE_GET_TYPE(Double);
