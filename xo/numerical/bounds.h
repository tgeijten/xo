#pragma once

#include "xo/xo_types.h"
#include "xo/container/prop_node_cast.h"
#include "xo/string/string_cast.h"
#include "xo/numerical/math.h"

namespace xo
{
	template< typename T >
	class bounds
	{
	public:
		bounds() : lower(), upper() {};
		bounds( const prop_node& props );
		template< typename U > bounds( const U& lower_bound, const U& upper_bound ) : lower( T( lower_bound ) ), upper( T( upper_bound ) ) {};
		template< typename U > bounds( const bounds< U >& other ) : lower( T( other.lower ) ), upper( T( other.upper ) ) {};
		template< typename U > bounds( const U& value ) : lower( T( value ) ), upper( T( value ) ) {};

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

	using boundsf = bounds< float >;
	using boundsd = bounds< double >;
	using boundsi = bounds< int >;

	template< typename T > string to_str( const bounds< T >& v ) {
		return to_str( v.lower ) + ' ' + to_str( v.upper );
	}

	template< typename T > xo::bounds<T>::bounds( const prop_node& pn ) {
		if ( pn.has_value() ) {
			lower = from_str( pn.get_value(), constants<T>::lowest() );
			auto p = pn.get_value().find( ".." );
			if ( p != string::npos )
				upper = from_str( pn.get_value().substr( p + 2 ), constants<T>::max() );
			else upper = lower; // single value
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
