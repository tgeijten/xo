#pragma once
#include "math.h"

namespace xo
{
	template< typename T >
	class bounds
	{
	public:
		template< typename T2 > bounds( const T2& lower_bound, const T2& upper_bound ) : lower( T( lower_bound ) ), upper( T( upper_bound ) ) {};
		template< typename T2 > bounds( const bounds< T2 >& other ) : lower( T( other.lower ) ), upper( T( other.upper ) ) {};
		template< typename T2 > bounds( const T2& value ) : lower( T( value ) ), upper( T( value ) ) {};
		bounds() : lower( T() ), upper( T() ) {};
		bounds( const prop_node& props );

		bool is_within( const T& value ) const { return ( value >= lower ) && ( value <= upper ); }
		T range() const { return upper - lower; }

		/// returns negative when value is below lower, positive when value is above upper, 0 when within bounds
		T get_violation( const T& value ) const { if ( value < lower ) return value - lower; else if ( value > upper ) return value - upper; else return T( 0 ); }

		T& clamp( T& value ) const { return clamp( value, lower, upper ); }
		T& soft_clamp( T& value, const T& boundary ) const { return soft_clamp( value, lower, upper, boundary ); }

		// return inverted bounds
		bounds< T > operator-() const { return bounds( -upper, -lower ); }

		T lower;
		T upper;
	};

	using boundsf = bounds< float >;
	using boundsd = bounds< double >;
	using boundsi = bounds< int >;

	template< typename T > std::ostream& operator<<( std::ostream& str, const bounds< T >& v ) {
		return str << v.lower << ' ' << v.upper;
	}

	template< typename T > char_stream& operator>>( char_stream& str, bounds< T >& v ) {
		return str >> v.lower >> v.upper;
	}

	template< typename T > xo::bounds<T>::bounds( const prop_node& pn ) {
		if ( pn.has_value() ) {
			lower = from_str< T >( pn.get_value() );
			auto p = pn.get_value().find( ".." );
			if ( p != string::npos )
				upper = from_str< T >( pn.get_value().substr( p + 2 ) );
			else upper = lower;
		}
		else if ( pn.size() > 0 ) {
			lower = pn.get_any< T >( { "min", "lower" }, constants<T>::lowest() );
			upper = pn.get_any<T>( { "max", "upper" }, constants< T >::max() );
		}
		else xo_error( "Cannot read bounds from prop_node" );
	}
}
