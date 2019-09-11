#pragma once

#include "mat33_type.h"
#include "vec3_type.h"
#include "quat_type.h"

namespace xo
{
	template< typename T >
	mat33_<T> mat33_from_axes( const vec3_<T>& x, const vec3_<T>& y, const vec3_<T>& z )
	{ return mat33_<T>( x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z ); }

	template< typename T >
	mat33_<T> mat33_from_diagonal( const vec3_<T>& d )
	{ return mat33_<T>( d.x, T(0), T( 0 ), T( 0 ), d.y, T( 0 ), T( 0 ), T( 0 ), d.z ); }

	/// Get quaternion from mat33
	template< typename T > quat_<T> quat_from_mat33( const mat33_<T>& m ) {
		quat_<T> q;
		T t = m.e00 + m.e11 + m.e22;
		if ( t > 0.0 ) {
			T root = sqrt( t + T( 1 ) );
			q.w = T( 0.5 ) * root;
			root = T( 0.5 ) / root;
			q.x = ( m.e21 - m.e12 ) * root;
			q.y = ( m[ 0 ][ 2 ] - m[ 2 ][ 0 ] ) * root;
			q.z = ( m[ 1 ][ 0 ] - m[ 0 ][ 1 ] ) * root;
		}
		else {
			size_t i = 0;
			if ( m[ 1 ][ 1 ] > m[ 0 ][ 0 ] ) i = 1;
			if ( m[ 2 ][ 2 ] > m[ i ][ i ] ) i = 2;
			size_t j = ( i + 1 ) % 3;
			size_t k = ( j + 1 ) % 3;
			T root = sqrt( m[ i ][ i ] - m[ j ][ j ] - m[ k ][ k ] + T( 1 ) );
			T* q_xyz = &q.x;
			q_xyz[ i ] = T( 0.5 ) * root;
			root = T( 0.5 ) / root;
			q.w = ( m[ k ][ j ] - m[ j ][ k ] ) * root;
			q_xyz[ j ] = ( m[ j ][ i ] + m[ i ][ j ] ) * root;
			q_xyz[ k ] = ( m[ k ][ i ] + m[ i ][ k ] ) * root;
		}

		return q;
	}
}
