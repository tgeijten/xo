#pragma once

#include <limits> // needed for std::numeric_limits; #todo: remove
#include <type_traits> // needed for std::is_same_v(); #todo: remove

namespace xo
{
	inline constexpr long double XO_PI_LONG_DOUBLE =	3.14159265358979323846L;
	inline constexpr long double XO_E_LONG_DOUBLE =		2.71828182845904523536L;
	inline constexpr long double XO_SQRT2_LONG_DOUBLE = 1.41421356237309504880L;

	// numerical constants via constexpr functions (pre C++17), will be deprecated
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
		static constexpr T infinity() { return std::numeric_limits<T>::infinity(); }
		static constexpr bool is_signed() { return std::numeric_limits<T>::is_signed; }
		static constexpr T sentinel() {
			static_assert( !std::is_same_v<T, bool>, "xo::constants<bool>::sentinal() is not supported" );
			if constexpr ( constants<T>::is_signed() )
				return constants<T>::lowest();
			else return constants<T>::max();
		}
	};

	// numerical constants via static constexpr values (C++17)
	template< typename T >
	struct num {
		static constexpr T pi = T( XO_PI_LONG_DOUBLE );
		static constexpr T half_pi = T( 0.5 ) * T( XO_PI_LONG_DOUBLE );
		static constexpr T two_pi = T( 2 ) * T( XO_PI_LONG_DOUBLE );
		static constexpr T e = T( XO_E_LONG_DOUBLE );
		static constexpr T sqrt2 = T( XO_SQRT2_LONG_DOUBLE );
		static constexpr T half_sqrt2 = T( 0.5 ) * T( XO_SQRT2_LONG_DOUBLE );
		static constexpr T epsilon = std::numeric_limits<T>::epsilon();
		static constexpr T ample_epsilon = T( 8 * std::numeric_limits<T>::epsilon() );
		static constexpr T max = std::numeric_limits<T>::max();
		static constexpr T highest = std::numeric_limits<T>::max();
		static constexpr T smallest = std::numeric_limits<T>::min();
		static constexpr T lowest = std::numeric_limits<T>::lowest();
		static constexpr T quiet_NaN = std::numeric_limits<T>::quiet_NaN();
		static constexpr T infinity = std::numeric_limits<T>::infinity();
		static constexpr bool is_signed = std::numeric_limits<T>::is_signed;
		static constexpr T sentinel() {
			static_assert( !std::is_same_v<T, bool>, "xo::num<bool>::sentinal() is not supported" );
			if constexpr ( constants<T>::is_signed() )
				return constants<T>::lowest();
			else return constants<T>::max();
		}
	};

	using constantsd = constants<double>;
	using constantsf = constants<float>;
	using constantsi = constants<int>;

	using numd = num<double>;
	using numf = num<float>;
	using numi = num<int>;
}
