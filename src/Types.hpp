
#pragma once

#include "Operand.hpp"

enum class eOperandType
{
    Int8,
    Int16,
    Int32,
    Float,
    Double,
    Count
};


using Int8 = Operand<int8_t>;
using Int16 = Operand<int16_t>;
using Int32 = Operand<int32_t>;
using Float = Operand<float>;
using Double = Operand<double>;

