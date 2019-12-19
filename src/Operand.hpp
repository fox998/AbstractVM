
#pragma once

#include <sstream>

#include "IOperand.hpp"

template<class T>
class Operand : public IOperand
{
    using Super = IOperand;
public:
    using value_type = T;

    IOperand const * operator+( IOperand const & rhs ) const override { return &rhs; }; // Sum
    IOperand const * operator-( IOperand const & rhs ) const override { return &rhs; }; // Difference
    IOperand const * operator*( IOperand const & rhs ) const override { return &rhs; }; // Product
    IOperand const * operator/( IOperand const & rhs ) const override { return &rhs; }; // Quotient

    //Operand<T> operator%(Operand<T> const& other) const;

    Operand() = default;
    Operand(Operand<T>&&) = default;
    Operand<T>& operator=(Operand<T> const&) = default;
    virtual ~Operand() = default;

    Operand(std::string const& str);

private:

    value_type m_Value;

    // Can`t use for floating point types
    IOperand const * operator%( IOperand const & rhs ) const override
    {
        return &rhs;
    }; // Modulo
};

// template<typename T>
// Operand<T> Operand<T>::operator%(Operand<T> const& other) const {
//     Super::operator%(other);
//     return other;
// }

// #define DEL_OPETOR(TYPE) \
// template<> \
// Operand<TYPE> Operand<TYPE>::operator%(Operand<TYPE> const& other) const = delete;

// DEL_OPETOR(float)
// DEL_OPETOR(double)


template<typename T>
Operand<T>::Operand(std::string const& str)
{
    std::stringstream ss(str);
    ss >> m_Value;
}

