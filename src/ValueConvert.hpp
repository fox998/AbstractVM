#pragma once

#include <type_traits>
#include <limits>

template<typename T, typename U>
T cafe_cast(U value)
{
    using ValueLim = std::numeric_limits<T>;
    assertion(value <= ValueLim::max(), "Overflow on a value");
    assertion(value >= ValueLim::lowest(), "Underflow on a value");

    return static_cast<T>(value);
}

template<
    typename T,
    typename std::enable_if_t<std::is_integral<T>::value, int> = 0
>
T toValue(std::string const& str)
{
    auto const value = std::stoll(str);
    return cafe_cast<T>(value);
}

template<
    typename T,
    typename std::enable_if_t<std::is_floating_point<T>::value, int> = 0
>
T toValue(std::string const& str)
{
    auto const value = std::stold(str);
    return cafe_cast<T>(value);
}
