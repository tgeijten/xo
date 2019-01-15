#pragma once

#include <limits> // TODO: get rid of this header

namespace xo
{
	const long double XO_PI_LONG_DOUBLE = 3.141592653589793238L;
	const long double XO_E_LONG_DOUBLE = 2.71828182845904523536L;

	template< typename T >
	struct numconst
	{
		static constexpr T pi() { return T( XO_PI_LONG_DOUBLE ); }
		static constexpr T half_pi() { return T( 0.5 ) * T( XO_PI_LONG_DOUBLE ); }
		static constexpr T two_pi() { return T( 2 ) * T( XO_PI_LONG_DOUBLE ); }
		static constexpr T e() { return T( XO_E_LONG_DOUBLE ); }
		static constexpr T epsilon() { return std::numeric_limits< T >::epsilon(); }
		static constexpr T ample_epsilon() { return T( 4 * std::numeric_limits< T >::epsilon() ); }
		static constexpr T max() { return std::numeric_limits<T>::max(); }
		static constexpr T min() { return std::numeric_limits<T>::min(); }
		static constexpr T lowest() { return std::numeric_limits<T>::lowest(); }
		static constexpr T NaN() { return std::numeric_limits<T>::quiet_NaN(); }
		static constexpr T sentinel() { return std::numeric_limits<T>::is_signed ? lowest<T>() : max<T>(); }
	};

	using numconstd = numconst< double >;
	using numconstf = numconst< float >;
	using numconsti = numconst< int >;

	// TODO: depricate the following functions
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
	template< typename T > bool greater_or_equal( T v1, T v2, T e = const_ample_epsilon<T>() ) { return v1 > v2 || equal( v1, v2, e ); }
}
