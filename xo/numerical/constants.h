#pragma once

#include <limits> // TODO: get rid of this header

namespace xo
{
	const long double PI_LONG_DOUBLE = 3.141592653589793238L;

	template< typename T > constexpr T pi() { return T( PI_LONG_DOUBLE ); }
	template< typename T > constexpr T half_pi() { return T( 0.5 ) * T( PI_LONG_DOUBLE ); }
	template< typename T > constexpr T two_pi() { return T( 2 ) * T( PI_LONG_DOUBLE ); }

	template< typename T > constexpr T epsilon() { return std::numeric_limits< T >::epsilon(); }
	template< typename T > constexpr T relaxed_epsilon() { return T( 4 * std::numeric_limits< T >::epsilon() ); }

	template< typename T > constexpr T max() { return std::numeric_limits<T>::max(); }
	template< typename T > constexpr T min() { return std::numeric_limits<T>::min(); }
	template< typename T > constexpr T lowest() { return std::numeric_limits<T>::lowest(); }
	template< typename T > constexpr T nan() { return std::numeric_limits<T>::quiet_NaN(); }

	template< typename T > constexpr T sentinel() { return std::numeric_limits<T>::is_signed ? std::numeric_limits<T>::lowest() : std::numeric_limits<T>::max(); }

	/// compare floating point variables using relaxed_epsilon
	// TODO: move somewhere else (not math.h because we don't want to include constants there)
	template< typename T > bool equals( T v1, T v2, T e = relaxed_epsilon<T>() ) { return std::abs( v1 - v2 ) <= std::abs( v1 ) * e; }
}
