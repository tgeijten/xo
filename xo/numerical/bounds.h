#pragma once

#include "xo/xo_types.h"
#include "xo/numerical/math.h"

namespace xo
{
	template< typename T >
	class bounds
	{
	public:
		bounds() : lower(), upper() {};
		bounds( const T& lower_bound, const T& upper_bound ) : lower( lower_bound ), upper( upper_bound ) {};
		template< typename U > bounds( const U& lower_bound, const U& upper_bound ) : lower( T( lower_bound ) ), upper( T( upper_bound ) ) {};
		template< typename U > bounds( const bounds< U >& other ) : lower( T( other.lower ) ), upper( T( other.upper ) ) {};

		bool is_within( const T& value ) const { return ( value >= lower ) && ( value <= upper ); }
		T range() const { return upper - lower; }

		/// returns negative when value is below lower, positive when value is above upper, 0 when within bounds
		T get_violation( const T& value ) const { if ( value < lower ) return value - lower; else if ( value > upper ) return value - upper; else return T( 0 ); }

		T& clamp( T& value ) const { return xo::clamp( value, lower, upper ); }
		T clamped( T value ) const { return xo::clamped( value, lower, upper ); }
		T& soft_clamp( T& value, const T& boundary ) const { return xo::soft_clamp( value, lower, upper, boundary ); }
		T soft_clamped( T value ) const { return xo::soft_clamped( value, lower, upper ); }

		// return inverted bounds
		bounds< T > operator-() const { return bounds( -upper, -lower ); }
		bool is_null() const { return lower == T( 0 ) && upper == T( 0 ); }

		T lower;
		T upper;
	};

	using boundsf = bounds<float>;
	using boundsd = bounds<double>;
	using boundsi = bounds<int>;
}
