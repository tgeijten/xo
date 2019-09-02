#pragma once

#include <limits> // #todo: get rid of this header

namespace xo
{
	constexpr long double XO_PI_LONG_DOUBLE = 3.141592653589793238L;
	constexpr long double XO_E_LONG_DOUBLE = 2.71828182845904523536L;

	// #todo: change to constexpr variables
	template< typename T >
	struct constants
	{
		static constexpr T pi() { return T( XO_PI_LONG_DOUBLE ); }
		static constexpr T half_pi() { return T( 0.5 ) * T( XO_PI_LONG_DOUBLE ); }
		static constexpr T two_pi() { return T( 2 ) * T( XO_PI_LONG_DOUBLE ); }
		static constexpr T e() { return T( XO_E_LONG_DOUBLE ); }
		static constexpr T epsilon() { return std::numeric_limits< T >::epsilon(); }
		static constexpr T ample_epsilon() { return T( 8 * std::numeric_limits< T >::epsilon() ); }
		static constexpr T max() { return std::numeric_limits<T>::max(); }
		static constexpr T min() { return std::numeric_limits<T>::min(); }
		static constexpr T lowest() { return std::numeric_limits<T>::lowest(); }
		static constexpr T NaN() { return std::numeric_limits<T>::quiet_NaN(); }
		static constexpr bool is_signed() { return std::numeric_limits<T>::is_signed; }
		static constexpr T sentinel() { return constants<T>::is_signed() ? constants<T>::lowest() : constants<T>::max(); }
	};

	using constantsd = constants< double >;
	using constantsf = constants< float >;
	using constantsi = constants< int >;
}
