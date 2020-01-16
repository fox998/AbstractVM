
#pragma once

#include <sstream>
#include <functional>
#include <memory>
#include <type_traits>

#include "IOperand.hpp"
#include "TypesFabric.hpp"
#include "Exception.hpp"
#include "ValueConvert.hpp"



template<class T>
class Operand : public IOperand
{
    using Super = IOperand;

public:
    using value_type = T;

    IOperand const * operator+( IOperand const & rhs ) const override;
    IOperand const * operator-( IOperand const & rhs ) const override;
    IOperand const * operator*( IOperand const & rhs ) const override;
    IOperand const * operator/( IOperand const & rhs ) const override;
    IOperand const * operator%( IOperand const & rhs ) const override;

    IOperand const * processOperation( std::string const& other, std::function<T(T, T)>&& valueOperator) const
    {
        auto const other_value = toValue<value_type>(other);
        value_type const end_value = valueOperator(m_valueNum, other_value);
        return TypesFabric::GetInstance().createOperand(getType(), std::to_string(end_value));
    };

    Operand() : Operand("0") {};
    Operand(std::string const& str);
    Operand(Operand<T>&&) = default;
    Operand<T>& operator=(Operand<T> const&) = default;
    virtual ~Operand() = default;


    std::string const & toString( void ) const override { return m_value; };
    T getValue() const { return m_valueNum; }
    eOperandType getType( void ) const override { return eOperandType::Invalid; };
    int getPrecision( void ) const override;

private:

    value_type const m_valueNum;
    std::string const m_value;
};


template<typename T>
Operand<T>::Operand(std::string const& str)
: m_valueNum( toValue<T>(str) )
, m_value( std::to_string(m_valueNum) )
{
}


template<typename T>
int Operand<T>::getPrecision( void ) const
{
    return static_cast<decltype(getPrecision())>(getType());
};



template<typename T>
IOperand const * Operand<T>::operator+( IOperand const & rhs ) const
{
    auto const higherType = static_cast<eOperandType>(std::max(getPrecision(), rhs.getPrecision()));

    return (higherType == getType() ?
        processOperation(rhs.toString(), [](auto a, auto b) { return a + b;} ) :
        rhs + *this // swap operands
    );
};

template<typename T>
IOperand const * Operand<T>::operator*( IOperand const & rhs ) const
{
    auto const higherType = static_cast<eOperandType>(std::max(getPrecision(), rhs.getPrecision()));

    return (higherType == getType() ?
        processOperation(rhs.toString(), [](auto a, auto b) { return a * b;} ) :
        rhs * *this // swap operands
    );
}; // Product


template<typename T>
IOperand const * Operand<T>::operator-( IOperand const & rhs ) const
{
    auto const higherType = static_cast<eOperandType>(std::max(getPrecision(), rhs.getPrecision()));

    if (higherType == getType())
    {
        return processOperation(rhs.toString(), [](auto a, auto b) { return a - b;} );
    }

    std::unique_ptr<IOperand const> const tmpOperand {
        TypesFabric::GetInstance().createOperand(rhs.getType(), "-" + rhs.toString())
    };
    return *tmpOperand + *this;
};


template<typename T>
IOperand const * Operand<T>::operator/( IOperand const & rhs ) const
{
    auto const higherType = static_cast<eOperandType>(std::max(getPrecision(), rhs.getPrecision()));

    assertion(
        higherType >= eOperandType::Float || rhs.toString() != "0",
        "Division by zero for integer type"    
    );

    if (higherType == getType())
    {
        return processOperation(rhs.toString(), [](auto a, auto b) { return a / b;} );
    }

    std::unique_ptr<IOperand const> const tmpOperand {
        TypesFabric::GetInstance().createOperand(higherType, toString())
    };
    return *tmpOperand / rhs;
}; // Quotient



template<
    typename T,
    typename std::enable_if_t<std::is_integral<T>::value, int> = 0
>
IOperand const * modulo(Operand<T> const& first, IOperand const & second )
{
    auto const higherType = static_cast<eOperandType>(std::max(first.getPrecision(), second.getPrecision()));

    if (higherType == first.getType())
    {
        return first.processOperation(second.toString(), [](auto a, auto b) { assertion(b != 0, "mod by zero"); return a % b;} );
    }

    std::unique_ptr<IOperand const> const tmpOperand {
        TypesFabric::GetInstance().createOperand(higherType, first.toString())
    };
    return *tmpOperand % second;
}

template<
    typename T,
    typename std::enable_if_t<std::is_floating_point<T>::value, int> = 0
>
IOperand const * modulo(Operand<T> const& , IOperand const & )
{
    assertion(false, "Can not process modulo operator for floting point types");
    return nullptr;
}

template<typename T>
IOperand const * Operand<T>::operator%( IOperand const & rhs ) const
{
    return modulo(*this, rhs);
};

