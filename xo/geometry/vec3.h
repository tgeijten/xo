#pragma once

#include "vec3_type.h"
#include "xo/numerical/compare.h"
#include "xo/numerical/math.h"
#include "xo/container/prop_node.h"
#include <cmath>

namespace xo
{
	/// Addition
	template< typename T > vec3_<T> operator+( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return vec3_<T>( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
	/// Addition
	template< typename T > vec3_<T>& operator+=( vec3_<T>& v1, const vec3_<T>& v2 )
	{ v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; return v1; }

	/// Subtraction
	template< typename T > vec3_<T> operator-( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return vec3_<T>( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	/// Subtraction
	template< typename T > vec3_<T>& operator-=( vec3_<T>& v1, const vec3_<T>& v2 )
	{ v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }

	/// Negation
	template< typename T > vec3_<T> operator-( vec3_<T> v )
	{ v.x = -v.x; v.y = -v.y; v.z = -v.z; return v; }

	/// Scalar multiplication
	template< typename T, typename TS > vec3_<T>& operator*=( vec3_<T>& v, TS s )
	{ v.x *= T(s); v.y *= T(s); v.z *= T(s); return v; }
	/// Scalar multiplication
	template< typename T, typename TS > vec3_<T> operator*( TS s, vec3_<T> v )
	{ return v *= s; }
	/// Scalar multiplication
	template< typename T, typename TS > vec3_<T> operator*( vec3_<T> v, TS s )
	{ return v *= s; }

	/// Scalar division
	template< typename T > vec3_<T> operator/( vec3_<T> v, T s )
	{ T ms = inv( s ); v.x *= ms; v.y *= ms; v.z *= ms; return v; }
	/// Invert components
	template< typename T > vec3_<T> operator/( T s, vec3_<T> v )
	{ v.x = s / v.x; v.y = s / v.y; v.z = s / v.z; return v; }
	/// Scalar division
	template< typename T > vec3_<T>& operator/=( vec3_<T>& v, T s )
	{ T ms = inv( s ); v.x *= ms; v.y *= ms; v.z *= ms; return v; }

	/// Scale components (masking)
	template< typename T > vec3_<T> operator*( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return vec3_<T>( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
	/// Scale components (masking)
	template< typename T > vec3_<T>& operator*=( vec3_<T>& v1, const vec3_<T>& v2 )
	{ v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }

	/// Inverse scale components
	template< typename T > vec3_<T> operator/( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return vec3_<T>( v1.x / v2.x, v1.y / v2.y, v1.z / v2.z ); }
	/// Inverse scale components
	template< typename T > vec3_<T>& operator/=( vec3_<T>& v1, const vec3_<T>& v2 )
	{ v1.x /= v2.x; v1.y /= v2.y; v1.z /= v2.z; return v1; }

	/// Get length of a vec3
	template< typename T > T length( const vec3_<T>& v )
	{ return std::sqrt( v.x * v.x + v.y * v.y + v.z * v.z ); }

	/// Get squared length of a vec3
	template< typename T > T squared_length( const vec3_<T>& v )
	{ return v.x * v.x + v.y * v.y + v.z * v.z; }

	/// Get Manhattan length of a vec3
	template< typename T > T manhattan_length( const vec3_<T>& v )
	{ return std::abs( v.x ) + std::abs( v.y ) + std::abs( v.z ); }

	/// Test if a vec3 is of unit length
	template< typename T > bool is_normalized( const vec3_<T>& v )
	{ return equal( v.x * v.x + v.y * v.y + v.z * v.z, T(1) ); }

	/// Get distance between two vec3
	template< typename T > T distance( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return length( v1 - v2 ); }

	/// Get squared distance between two vec3
	template< typename T > T squared_distance( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return squared_length( v1 - v2 );	}

	/// Compare vec3
	template< typename T > bool operator==( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
	template< typename T > bool operator!=( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return !( v1 == v2 ); }

	/// Compare vec3 with epsilon
	template< typename T > bool equal( const vec3_<T>& v1, const vec3_<T>& v2, T e = constants<T>::ample_epsilon() )
	{ return equal( v1.x, v2.x, e ) && equal( v1.y, v2.y, e ) && equal( v1.z, v2.z, e ); }

	/// Normalize a vec3, returns length
	template< typename T > T normalize( vec3_<T>& v )
	{ T l = length( v ); if ( l > constants<T>::epsilon() ) v /= l; return l; }

	/// clamp a value so that it is between lb and ub. #perf: check special instruction
	template< typename T > vec3_<T>& clamp( vec3_<T>& v, const T& lb, const T& ub )
	{ clamp( v.x, lb, ub ); clamp( v.y, lb, ub ); clamp( v.z, lb, ub ); return v; }

	/// Get normalized vec3
	template< typename T > vec3_<T> normalized( vec3_<T> v ) { normalize( v ); return v; }

	/// Get absolute value of vec3
	template< typename T > vec3_<T> abs( vec3_<T> v ) { v.x = std::abs( v.x ); v.y = std::abs( v.y ); v.z = std::abs( v.z ); return v; }

	/// Dot product of two vec3
	template< typename T1, typename T2 > auto dot_product( const vec3_<T1>& v1, const vec3_<T2>& v2 )
	{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	/// Projection of v1 onto v2
	template< typename T > vec3_<T> projection( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return ( dot_product( v1, v2 ) / squared_length( v2 ) ) * v2; }

	/// Cross product
	template< typename T > vec3_<T> cross_product( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return vec3_<T>( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x ); }

	/// convert from prop_node
	template< typename T > bool from_prop_node( const prop_node& pn, vec3_<T>& v ) {
		v.set( pn.get( "x", T() ), pn.get( "y", T() ), pn.get( "z", T() ) );
		if ( pn.size() >= 3 && pn.is_array() ) {
			v.set( pn.get<T>( 0 ), pn.get<T>( 1 ), pn.get<T>( 2 ) );
		}
		else if ( pn.size() == 0 && pn.has_value() ) {
			if ( auto vs = split_str( pn.raw_value(), " \t" ); vs.size() >= 3 )
				return from_str( vs[ 0 ], v.x ) && from_str( vs[ 1 ], v.y ) && from_str( vs[ 2 ], v.z );
			else return false;
		}
		return true;
	};

	/// convert to prop_node
	template< typename T > prop_node to_prop_node( const vec3_<T>& v ) {
		prop_node pn;
		pn.set( "x", v.x );
		pn.set( "y", v.y );
		pn.set( "z", v.z );
		return pn;
	}
}
