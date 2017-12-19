#pragma once

#include "vec_type.h"
#include "xo/numerical/math.h"
#include "xo/system/assert.h"

namespace xo
{
	/// Addition
	template< typename T > vec_<T> operator+( const vec_<T>& v1, const vec_<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		vec_<T> r( v1.size() );
		for ( index_t i = 0; i < r.size(); ++i )
			r[ i ] = v1[ i ] + v2[ i ];
		return r;
	}
	/// Addition
	template< typename T > vec_<T>& operator+=( vec_<T>& v1, const vec_<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		for ( index_t i = 0; i < r.size(); ++i )
			v1[ i ] += v2[ i ];
		return v1;
	}

	/// Subtraction
	template< typename T > vec_<T> operator-( const vec_<T>& v1, const vec_<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		vec_<T> r( v1.size() );
		for ( index_t i = 0; i < r.size(); ++i )
			r[ i ] = v1[ i ] - v2[ i ];
		return r;
	}
	/// Subtraction
	template< typename T > vec_<T>& operator-=( vec_<T>& v1, const vec_<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		for ( index_t i = 0; i < r.size(); ++i )
			v1[ i ] -= v2[ i ];
		return v1;
	}

	/// Negation
	template< typename T > vec_<T> operator-( vec_<T> v )
	{
		for ( auto& e : v )
			e = -e;
		return v;
	}

	/// Scalar multiplication
	template< typename T, typename TS > vec_<T> operator*( TS s, vec_<T> v )
	{
		for ( auto& e : v )
			e *= s;
		return v;
	}
	/// Scalar multiplication
	template< typename T, typename TS > vec_<T> operator*( vec_<T> v, TS s )
	{
		for ( auto& e : v )
			e *= s;
		return v;
	}
	/// Scalar multiplication
	template< typename T, typename TS > vec_<T>& operator*=( vec_<T>& v, TS s )
	{
		for ( auto& e : v )
			e *= s;
		return v;
	}

	/// Scalar division
	template< typename T > vec_<T> operator/( vec_<T> v, T s )
	{
		return v * inv( s );
	}
	/// Scalar division
	template< typename T > vec_<T>& operator/=( vec_<T>& v, T s )
	{
		return v *= inv( s );
	}

	/// Test if a vec3 is of unit length
	template< typename T > T is_normalized( const vec_<T>& v )
	{
		return equals( v.length(), T( 1 ) );
	}

	/// Get distance between two vec
	template< typename T > T distance( const vec_<T>& v1, const vec_<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		T sum = T( 0 );
		for ( index_t i = 0; i < v1.size(); ++i )
			sum += squared( v1[ i ] - v2[ i ] );
		return sqrt( sum );
	}

	/// Normalize a vec3
	template< typename T > void normalize( vec_<T>& v )
	{
		T l = length( v ); if ( l > constants<T>::epsilon() ) { v /= l; }
	}

	/// Get normalized vec3
	template< typename T > vec_<T> normalized( vec_<T> v )
	{
		normalize( v ); return v;
	}

	/// Dot product of two vec3
	template< typename T > T dot_product( const vec_<T>& v1, const vec_<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		T r = T( 0 );
		for ( index_t i = 0; i < v1.size(); ++i )
			r += v1[ i ] * v2[ i ];
		return r;
	}

	/// stream out
	template< typename T > std::ostream& operator<<( std::ostream& str, const vec_<T>& v )
	{
		if ( v.size() > 0 )
		{
			str << v[ 0 ];
			for ( index_t i = 1; i < v.size(); ++i )
				str << ' ' << v[ i ];
		}
		return str;
	}

	/// stream in
	template< typename T > std::istream& operator >> ( std::istream& str, vec_<T>& v )
	{
		for ( auto& e : v )
			str >> e;
		return str;
	}
}
