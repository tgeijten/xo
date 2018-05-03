#pragma once

#include "vec3_type.h"
#include "xo/numerical/math.h"
#include "angle.h"

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
	template< typename T, typename TS > vec3_<T> operator*( TS s, vec3_<T> v )
	{ v.x *= s; v.y *= s; v.z *= s; return v; }
	/// Scalar multiplication
	template< typename T, typename TS > vec3_<T> operator*( vec3_<T> v, TS s )
	{ v.x *= s; v.y *= s; v.z *= s; return v; }
	/// Scalar multiplication
	template< typename T, typename TS > vec3_<T>& operator*=( vec3_<T>& v, TS s )
	{ v.x *= s; v.y *= s; v.z *= s; return v; }

	/// Scalar division
	template< typename T > vec3_<T> operator/( vec3_<T> v, T s )
	{ T ms = inv( s ); v.x *= ms; v.y *= ms; v.z *= ms; return v; }
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
	{ return sqrt( v.x * v.x + v.y * v.y + v.z * v.z ); }

	/// Get squared length of a vec3
	template< typename T > T squared_length( const vec3_<T>& v )
	{ return v.x * v.x + v.y * v.y + v.z * v.z; }

	/// Test if a vec3 is of unit length
	template< typename T > T is_normalized( const vec3_<T>& v )
	{ return equals( v.x * v.x + v.y * v.y + v.z * v.z, T(1) ); }

	/// Get distance between two vec3
	template< typename T > T distance( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return sqrt( squared( v1.x - v2.x ) + squared( v1.y - v2.y ) + squared( v1.z - v2.z ) ); }

	/// compare vec3
	template< typename T > bool operator==( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
	template< typename T > bool operator!=( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return !( v1 == v2 ); }

	/// Normalize a vec3
	template< typename T > void normalize( vec3_<T>& v )
	{ T l = length( v ); if ( l > constants<T>::epsilon() ) { T s = inv( l ); v.x *= s; v.y *= s; v.z *= s; } }

	/// Get normalized vec3
	template< typename T > vec3_<T> normalized( vec3_<T> v )
	{ normalize( v ); return v; }

	/// Dot product of two vec3
	template< typename T > T dot_product( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	/// Cross product
	template< typename T > vec3_<T> cross_product( const vec3_<T>& v1, const vec3_<T>& v2 )
	{ return vec3_<T>( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x ); }

	/// Make vec3 from radius, inclination & azimuth
	template< angle_unit U, typename T > vec3_<T> vec3_from_polar( T radius, angle_<U, T> inclination, angle_<U, T> azimuth )
	{ return vec3_<T>( radius * sin( inclination ) * sin( azimuth ) , radius * cos( inclination ), radius * sin( inclination ) * cos( azimuth ) ); }

	/// stream out
	template< typename T > std::ostream& operator<<( std::ostream& str, const vec3_<T>& v )
	{ str << v.x << ' ' << v.y << ' ' << v.z; return str; }

	/// stream in
	template< typename T > std::istream& operator>>( std::istream& str, vec3_<T>& v )
	{ str >> v.x >> v.y >> v.z; return str; }
}
