#pragma once

#include <string>
#include <memory>

#include "Types.hpp"

class IOperand;


class TypesFabric
{
    using OperantVraper = IOperand const* ;// std::unique_ptr<IOperand const>;

    OperantVraper createInt8( std::string const & value ) const;
    OperantVraper createInt16( std::string const & value ) const;
    OperantVraper createInt32( std::string const & value ) const;
    OperantVraper createFloat( std::string const & value ) const;
    OperantVraper createDouble( std::string const & value ) const;

    using BuilderType = decltype(&TypesFabric::createInt8);
    BuilderType GetBuilder(eOperandType type) const;

public:

    static TypesFabric const& GetInstance();

    OperantVraper createOperand( eOperandType type, std::string const & value ) const;
};
