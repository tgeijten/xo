#pragma once

#include "xo/serialization/serialize.h"

namespace xo
{
	template< typename T >
	class bounds
	{
	public:
		bounds() : lower(), upper() {};
		bounds( const prop_node& props );
		template< typename T2 > bounds( const T2& lower_bound, const T2& upper_bound ) : lower( T( lower_bound ) ), upper( T( upper_bound ) ) {};
		template< typename T2 > bounds( const bounds< T2 >& other ) : lower( T( other.lower ) ), upper( T( other.upper ) ) {};

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

		T lower;
		T upper;
	};

	using boundsf = bounds< float >;
	using boundsd = bounds< double >;
	using boundsi = bounds< int >;

	template< typename T > std::ostream& operator<<( std::ostream& str, const bounds< T >& v ) {
		return str << v.lower << ' ' << v.upper;
	}

	template< typename T > std::istream& operator>>( std::istream& str, bounds< T >& v ) {
		return str >> v.lower >> v.upper;
	}

	template< typename T > char_stream& operator>>( char_stream& str, bounds< T >& v ) {
		return str >> v.lower >> v.upper;
	}

	template< typename T > xo::bounds<T>::bounds( const prop_node& pn ) {
		if ( pn.has_value() ) {
			lower = from_str< T >( pn.get_value(), const_lowest<T>() );
			auto p = pn.get_value().find( ".." );
			if ( p != string::npos )
				upper = from_str< T >( pn.get_value().substr( p + 2 ), const_max<T>() );
			else upper = lower;
		}
		else if ( pn.size() >= 2 ) {
			lower = pn.get_any< T >( { "min", "lower" }, pn.get< T >( 0 ) );
			upper = pn.get_any<T>( { "max", "upper" }, pn.get< T >( 1 ) );
		}
		else xo_error( "Cannot read bounds from prop_node" );
	}

	// TODO: make a generic version of IS_PROP_NODE_CONSTRUCTABLE;
	IS_PROP_NODE_CONSTRUCTABLE( boundsd );
	IS_PROP_NODE_CONSTRUCTABLE( boundsf );
	IS_PROP_NODE_CONSTRUCTABLE( boundsi );
}
