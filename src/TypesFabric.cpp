#include "TypesFabric.hpp"

#include <assert.h>
#include <map>

#include "IOperand.hpp"
#include "Operand.hpp"
#include "Types.hpp"


#define DECLERE_BUILDER(TYPE)\
TypesFabric::OperantVraper TypesFabric::create##TYPE( std::string const & value ) const \
{\
    return std::make_unique<TYPE>(value); \
}

DECLERE_BUILDER(Int8);
DECLERE_BUILDER(Int16);
DECLERE_BUILDER(Int32);
DECLERE_BUILDER(Float);
DECLERE_BUILDER(Double);

TypesFabric::BuilderType TypesFabric::GetBuilder(eOperandType type) const
{
    assert(type < eOperandType::Count);

    #define MAPVALUE(TYPE) std::make_pair(eOperandType::TYPE, &TypesFabric::create##TYPE)
    const std::map<eOperandType, BuilderType> buildersFunctions = {
        MAPVALUE(Int8),
        MAPVALUE(Int16),
        MAPVALUE(Int32),
        MAPVALUE(Float),
        MAPVALUE(Double)
    };

    return buildersFunctions.at(type);
}


TypesFabric const& TypesFabric::GetInstance()
{
    static const TypesFabric Instance;
    return Instance;
}

TypesFabric::OperantVraper TypesFabric::createOperand( eOperandType type, std::string const & value ) const
{
    auto const& builder = GetBuilder(type);
    return (this->*builder)(value) ;
}