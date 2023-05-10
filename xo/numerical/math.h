#pragma once

#include "xo/xo_types.h"
#include <cmath>

namespace xo
{
	/// min
	template< typename T > const T& min( const T& v1, const T& v2 ) { return v1 < v2 ? v1 : v2; }
	template< typename T > const T& min( const T& v1, const T& v2, const T& v3 ) { return min( v1, min( v2, v3 ) ); }

	/// max
	template< typename T > const T& max( const T& v1, const T& v2 ) { return v1 < v2 ? v2 : v1; }
	template< typename T > const T& max( const T& v1, const T& v2, const T& v3 ) { return max( v1, max( v2, v3 ) ); }

	/// abs
	template< typename T > T abs( T v ) { return v < T( 0 ) ? -v : v; }

	/// inverse of v (1 / v)
	template< typename T > T inv( T v ) { return T( 1 ) / v; }

	/// inverse of sqrt
	template< typename T > T invsqrt( T v ) { return T( 1 ) / sqrt( v ); }

	/// sqrt based on sign: if v >= 0 sqrt( v ) else -sqrt( -v )
	template< typename T > T signed_sqrt( T v ) { return v >= T() ? sqrt( v ) : -sqrt( -v ); }

	/// squared
	template< typename T > T squared( T v ) { return v * v; }

	/// cubed
	template< typename T > T cubed( T v ) { return v * v * v; }

	/// midpoint between v1 and v2
	template< typename T > T midpoint( const T& v1, const T& v2 ) { return v1 + ( v2 - v1 ) / 2; }

	/// sign of v: 1 or -1
	template< typename T > T sign( T v ) { return v >= T() ? T( 1 ) : T( -1 ); }

	/// signum of v: 1, 0 or -1
	template< typename T > int signum( T v ) { return ( T( 0 ) < v ) - ( v < T( 0 ) ); }

	/// check if sign of v1 equals sign of v2
	template< typename T > bool same_sign( T v1, T v2 ) { return signum( v1 ) == signum( v2 ); }

	/// set if smaller
	template< typename T > T& set_if_smaller( T& value, const T& other ) { if ( other < value ) value = other; return value; }

	/// set if bigger
	template< typename T > T& set_if_bigger( T& value, const T& other ) { if ( other > value ) value = other; return value; }

	/// set if different, returns true if set
	template< typename T > bool set_if_different( T& value, const T& other ) { if ( other == value ) return false; value = other; return true; }

	/// check if an integer value is a power of two
	template< typename T > bool is_power_of_two( T v ) { return v != 0 && !( v & ( v - 1 ) ); }

	/// check if an integer value is odd / even
	template< typename T > bool is_even( T v ) { return ( v & 1 ) == 0; }
	template< typename T > bool is_odd( T v ) { return ( v & 1 ) == 1; }

	/// check if a number is whole (i.e. has no decimals)
	template< typename T > bool is_whole( T v ) { return floor( v ) == v; }

	/// round and cast to integer type
	template< typename TI, typename TF >
	TI round_cast( TF value ) { return static_cast<TI>( std::round( value ) ); }

	/// round to number of decimals
	template< typename T >
	T round_decimals( T value, int decimals ) { auto f = std::pow( T( 10 ), T( decimals ) ); return std::round( value * f ) / f; }

	/// clamp a value so that it is between min and max
	template< typename T > T& clamp( T& v, const T& lower, const T& upper )
	{ if ( v < lower ) v = lower; else if ( v > upper ) v = upper; return v; }
	// { return v = std::max( std::min( v, upper ), lower ); } // seems slower, despite https://godbolt.org/z/dZ2nXJ 

	/// return clamped value that is between min and max
	template< typename T > T clamped( T v, const T& lower, const T& upper )
	{ return clamp( v, lower, upper ); }
	// { return std::max( std::min( v, upper ), lower ); } // seems slower, despite https://godbolt.org/z/dZ2nXJ 

	/// limit transform function, used by soft_clamp
	template< typename T > T limit_transform( T x, T limit ) { return limit - limit * limit / ( x + limit ); }

	/// clamp with smooth boundary transformation
	template< typename T > T soft_clamp( T& v, const T& min, const T& max, const T& boundary ) {
		auto r = boundary * ( max - min );
		if ( v > max - r ) v = max - r + limit_transform( v - max + r, r );
		else if ( v < min + r )	v = min + r - limit_transform( min + r - v, r );
		return v;
	}

	template< typename T > T soft_clamped( T v, const T& min, const T& max, const T& boundary )
	{ return soft_clamp( v, min, max, boundary ); }

	/// convert float [0..1] to uint [0..255]
	template< typename T >
	uint32 float_to_byte( const T& value ) { return round_cast<uint32>( clamped( value, T( 0 ), T( 1 ) ) * T( 255 ) ); }

	/// convert uint32 [0..255] to float [0..1]
	template< typename T >
	T get_byte( T value, int start_bit ) { return ( value >> start_bit ) & T( 255 ); }

	/// wrap a value to a range
	template< typename T > T& wrap( T& v, const T& lower, const T& upper ) {
		if ( ( v - lower ) >= 0 )
			return v = std::fmod( v - lower, upper - lower ) + lower;
		else return v = std::fmod( v - lower, upper - lower ) + upper;
	}

	/// wrap a value to a range
	template< typename T > T wrapped( T v, const T& lower, const T& upper ) { return wrap( v, lower, upper ); }

	/// check a value is within a range
	template< typename T > bool is_between( T v, T min, T max ) { return ( v >= min && v <= max ); }

	/// square wave, period = 2, min = 0, max = 1
	template< typename T > T square_wave( T v ) {
		return std::fmod( v, T( 2 ) ) < T( 1 ) ? T( 1 ) : T( 0 );
	}

	/// triangle wave, period = 2, min = 0, max = 1
	template< typename T > T triangle_wave( T v ) {
		return T( 2 ) * std::abs( T( 0.5 ) * v - std::floor( T( 0.5 ) * v + T( 0.5 ) ) );
	}

	/// positive modulo
	template< typename T > T pos_mod( T x, T y ) {
		return fmod( x, y ) + T( x < 0 ) * y;
	}

	/// triangle wave between l and u, period = 2(u-l).
	template< typename T > T triangle_wave( T v, T l, T u ) {
		auto a = u - l;
		return std::abs( pos_mod( v - u, 2 * a ) - a ) + l;
	}

	/// greatest common divisor for integers (Euclid's algorithm)
	template< typename T > T greatest_common_divisor( T p, T q ) {
		while ( q ) { auto t = p % q; p = q; q = t; }
		return p;
	}
}
