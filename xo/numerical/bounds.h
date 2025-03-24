#pragma once

#include "xo/xo_types.h"
#include "xo/container/pair_type.h"
#include "xo/numerical/math.h"
#include "xo/numerical/constants.h"
#include "xo/container/prop_node.h"

namespace xo
{
	template< typename T >
	class bounds
	{
	public:
		constexpr bounds() : lower(), upper() {};
		constexpr bounds( const T& lower_bound, const T& upper_bound ) : lower( lower_bound ), upper( upper_bound ) {};
		template< typename U > constexpr bounds( const U& lower_bound, const U& upper_bound ) : lower( T( lower_bound ) ), upper( T( upper_bound ) ) {};
		template< typename U > constexpr bounds( const bounds< U >& other ) : lower( T( other.lower ) ), upper( T( other.upper ) ) {};

		bool is_within( const T& value ) const { return ( value >= lower ) && ( value <= upper ); }
		T length() const { return upper - lower; }
		T center() const { return T( 0.5 ) * ( lower + upper ); }

		/// check bounds, return negative when value below lower, positive when value above upper, 0 when within bounds
		T get_excess( const T& value ) const { if ( value < lower ) return value - lower; else if ( value > upper ) return value - upper; else return T( 0 ); }

		/// check bounds, return negative when value below lower, positive when value above upper, 0 when within bounds
		pair< bool, T > check( const T& value ) const {
			if ( value < lower )
				return { false, value - lower };
			else if ( value > upper )
				return { false, value - upper };
			else return { true, T( 0 ) };
		}

		void extend( const T& value ) { lower = min( lower, value ); upper = max( upper, value ); }
		void extend( const bounds& b ) { lower = min( lower, b.lower ); upper = max( upper, b.upper ); }

		T& clamp( T& value ) const { return xo::clamp( value, lower, upper ); }
		T clamped( T value ) const { return xo::clamped( value, lower, upper ); }
		T& soft_clamp( T& value, const T& boundary ) const { return xo::soft_clamp( value, lower, upper, boundary ); }
		T soft_clamped( T value ) const { return xo::soft_clamped( value, lower, upper ); }

		// return inverted bounds
		bounds< T > operator-() const { return bounds( -upper, -lower ); }
		bool is_null() const { return lower == T( 0 ) && upper == T( 0 ); }

		T lower;
		T upper;

		static constexpr bounds<T> infinite() { return bounds<T>( num<T>::lowest, num<T>::max ); }
		static constexpr bounds<T> positive() { return bounds<T>( T( 0 ), num<T>::max ); }
		static constexpr bounds<T> negative() { return bounds<T>( num<T>::lowest, T( 0 ) ); }
	};

	using boundsf = bounds<float>;
	using boundsd = bounds<double>;
	using boundsi = bounds<int>;

	/// convert bounds to string
	template< typename T > string to_str( const bounds<T>& v ) {
		return to_str( v.lower ) + ".." + to_str( v.upper );
	}

	/// convert bounds from string
	template< typename T > bool from_str( const string& s, bounds<T>& v ) {
		if ( from_str( s, v.lower ) ) {
			if ( auto p = s.find( ".." ); p != string::npos )
				return from_str( s.substr( p + 2 ), v.upper );
			else { v.upper = v.lower; return true; }
		}
		else return false;
	}

	/// convert bounds from prop_node
	template< typename T > bool from_prop_node( const prop_node& pn, bounds<T>& v ) {
		if ( pn.has_value() ) {
			return from_str( pn.get_str(), v );
		}
		else if ( pn.size() >= 2 ) {
			v.lower = pn.get_any<T>( { "min", "lower" }, pn.get<T>( 0 ) );
			v.upper = pn.get_any<T>( { "max", "upper" }, pn.get<T>( 1 ) );
			return true;
		}
		else return false;
	}
}
