#pragma once

#include "vec4_type.h"
#include "math.h"

namespace xo
{
	/// Addition
	template< typename T > vec4_<T> operator+( const vec4_<T>& v1, const vec4_<T>& v2 )
	{ return vec4_<T>( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w ); }
	/// Addition
	template< typename T > vec4_<T>& operator+=( vec4_<T>& v1, const vec4_<T>& v2 )
	{ v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; v1.w += v2.w; return v1; }

	/// Subtraction
	template< typename T > vec4_<T> operator-( const vec4_<T>& v1, const vec4_<T>& v2 )
	{ return vec4_<T>( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	/// Subtraction
	template< typename T > vec4_<T>& operator-=( vec4_<T>& v1, const vec4_<T>& v2 )
	{ v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }

	/// Negation
	template< typename T > vec4_<T> operator-( vec4_<T> v )
	{ v.x = -v.x; v.y = -v.y; v.z = -v.z; return v; }

	/// Scalar multiplication
	template< typename T > vec4_<T> operator*( T s, vec4_<T> v )
	{ v.x *= s; v.y *= s; v.z *= s; return v; }
	/// Scalar multiplication
	template< typename T > vec4_<T> operator*( vec4_<T> v, T s )
	{ v.x *= s; v.y *= s; v.z *= s; return v; }
	/// Scalar multiplication
	template< typename T > vec4_<T>& operator*=( vec4_<T>& v, T s )
	{ v.x *= s; v.y *= s; v.z *= s; return v; }

	/// Scalar division
	template< typename T > vec4_<T> operator/( vec4_<T> v, T s )
	{ T ms = inv( s ); v.x *= ms; v.y *= ms; v.z *= ms; return v; }
	/// Scalar division
	template< typename T > vec4_<T>& operator/=( vec4_<T>& v, T s )
	{ T ms = inv( s ); v.x *= ms; v.y *= ms; v.z *= ms; return v; }
}

