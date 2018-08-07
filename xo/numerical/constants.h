#pragma once

#include <limits> // TODO: get rid of this header

namespace xo
{
	const long double XO_PI_LONG_DOUBLE = 3.141592653589793238L;
	const long double XO_E_LONG_DOUBLE = 2.71828182845904523536L;

	template< typename T > constexpr T const_pi() { return T( XO_PI_LONG_DOUBLE ); }
	template< typename T > constexpr T const_pi_2() { return T( 0.5 ) * T( XO_PI_LONG_DOUBLE ); }
	template< typename T > constexpr T const_2_pi() { return T( 2 ) * T( XO_PI_LONG_DOUBLE ); }

	template< typename T > constexpr T const_e() { return T( XO_E_LONG_DOUBLE ); }

	template< typename T > constexpr T const_epsilon() { return std::numeric_limits< T >::epsilon(); }
	template< typename T > constexpr T const_ample_epsilon() { return T( 4 * std::numeric_limits< T >::epsilon() ); }

	template< typename T > constexpr T const_max() { return std::numeric_limits<T>::max(); }
	template< typename T > constexpr T const_min() { return std::numeric_limits<T>::min(); }
	template< typename T > constexpr T const_lowest() { return std::numeric_limits<T>::lowest(); }
	template< typename T > constexpr T const_NaN() { return std::numeric_limits<T>::quiet_NaN(); }

	template< typename T > constexpr T const_sentinel() { return std::numeric_limits<T>::is_signed ? const_lowest<T>() : const_max<T>(); }

	/// compare floating point variables using relaxed_epsilon
	// TODO: move somewhere else (not math.h because we don't want to include constants there)
	template< typename T > bool equal( T v1, T v2, T e = const_ample_epsilon<T>() ) { return std::abs( v1 - v2 ) <= std::abs( v1 ) * e; }
	template< typename T > bool less_or_equal( T v1, T v2, T e = const_ample_epsilon<T>() ) { return v1 < v2 || equal( v1, v2, e ); }
}
