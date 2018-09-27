#pragma once

#include <type_traits>

/// macro to hide hideous SNFINAE syntax
#define XO_ENABLE_IF_INTEGRAL typename std::enable_if_t< std::is_integral<T>::value >* = nullptr

/// macro to hide hideous SNFINAE syntax
#define XO_ENABLE_IF_FLOATING_POINT typename std::enable_if_t< std::is_floating_point<T>::value >* = nullptr

/// macro to hide hideous SNFINAE syntax
#define XO_ENABLE_IF_ARITHMETIC typename std::enable_if_t< std::is_arithmetic<T>::value >* = nullptr

/// macro to hide hideous SNFINAE syntax
#define XO_ENABLE_IF_ENUM typename std::enable_if_t< std::is_enum<T>::value >* = nullptr
