#pragma once

#include <exception>
#include <stdexcept>

//class CastomException : public std::exception {};

template<class Exception = std::runtime_error>
void assertion(bool const statement, char const* errorMsg = "")
{
    static_assert(std::is_base_of<std::exception, Exception>::value);
    if (statement == false)
    {
        throw Exception(errorMsg);
    }
}
