#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <type_traits>
#include <assert.h>


#include "Operand.hpp"


int main()
{
    Operand<float> a, b;

    a + b;
    //a % b;

    //auto a = Operand<float>();
    //auto a = Operand<int>();
}
