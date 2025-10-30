#pragma once

#include "mat33_type.h"
#include "vec3_type.h"
#include "quat_type.h"
#include <cmath>

namespace xo
{
	template< typename T >
	mat3_<T> mat3_from_axes( const vec3_<T>& x, const vec3_<T>& y, const vec3_<T>& z ) {
		return mat3_<T>( x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z );
	}

	template< typename T >
	mat3_<T> mat3_from_diagonal( const vec3_<T>& d ) {
		return mat3_<T>( d.x, T( 0 ), T( 0 ), T( 0 ), d.y, T( 0 ), T( 0 ), T( 0 ), d.z );
	}

	/// Matrix multiplication
	template< typename T > mat3_<T> operator*( const mat3_<T>& a, const mat3_<T>& b ) {
		return mat3_<T>(
			a.e00 * b.e00 + a.e01 * b.e10 + a.e02 * b.e20,
			a.e00 * b.e01 + a.e01 * b.e11 + a.e02 * b.e21,
			a.e00 * b.e02 + a.e01 * b.e12 + a.e02 * b.e22,
			a.e10 * b.e00 + a.e11 * b.e10 + a.e12 * b.e20,
			a.e10 * b.e01 + a.e11 * b.e11 + a.e12 * b.e21,
			a.e10 * b.e02 + a.e11 * b.e12 + a.e12 * b.e22,
			a.e20 * b.e00 + a.e21 * b.e10 + a.e22 * b.e20,
			a.e20 * b.e01 + a.e21 * b.e11 + a.e22 * b.e21,
			a.e20 * b.e02 + a.e21 * b.e12 + a.e22 * b.e22
			);
	}

	/// Vector multiplication
	template< typename T > vec3_<T> operator*( const mat3_<T>& m, const vec3_<T>& v ) {
		return vec3_<T>(
			m.e00 * v.x + m.e01 * v.y + m.e02 * v.z,
			m.e10 * v.x + m.e11 * v.y + m.e12 * v.z,
			m.e20 * v.x + m.e21 * v.y + m.e22 * v.z );
	}

	/// Get mat3 from quaternion
	template< typename T > mat3_<T> mat3_from_quat( const quat_<T>& q ) {
		mat3_<T> m{ no_init };

		auto xx = q.x * q.x;
		auto yy = q.y * q.y;
		auto zz = q.z * q.z;
		auto xy = q.x * q.y;
		auto xz = q.x * q.z;
		auto yz = q.y * q.z;
		auto wx = q.w * q.x;
		auto wy = q.w * q.y;
		auto wz = q.w * q.z;

		m.e00 = T( 1 ) - T( 2 ) * ( yy + zz );
		m.e01 = T( 2 ) * ( xy - wz );
		m.e02 = T( 2 ) * ( xz + wy );
		m.e10 = T( 2 ) * ( xy + wz );
		m.e11 = T( 1 ) - T( 2 ) * ( xx + zz );
		m.e12 = T( 2 ) * ( yz - wx );
		m.e20 = T( 2 ) * ( xz - wy );
		m.e21 = T( 2 ) * ( yz + wx );
		m.e22 = T( 1 ) - T( 2 ) * ( xx + yy );

		return m;
	}

	/// Get quaternion from mat3
	template< typename T > quat_<T> quat_from_mat3( const mat3_<T>& m ) {
		quat_<T> q;
		T t = m.e00 + m.e11 + m.e22;
		if ( t > 0.0 ) {
			T root = std::sqrt( t + T( 1 ) );
			q.w = T( 0.5 ) * root;
			root = T( 0.5 ) / root;
			q.x = ( m.e21 - m.e12 ) * root;
			q.y = ( m.e02 - m.e20 ) * root;
			q.z = ( m.e10 - m.e01 ) * root;
		}
		else {
			size_t i = 0;
			if ( m[1][1] > m[0][0] ) i = 1;
			if ( m[2][2] > m[i][i] ) i = 2;
			size_t j = ( i + 1 ) % 3;
			size_t k = ( j + 1 ) % 3;
			T root = std::sqrt( m[i][i] - m[j][j] - m[k][k] + T( 1 ) );
			T* q_xyz = &q.x;
			q_xyz[i] = T( 0.5 ) * root;
			root = T( 0.5 ) / root;
			q.w = ( m[k][j] - m[j][k] ) * root;
			q_xyz[j] = ( m[j][i] + m[i][j] ) * root;
			q_xyz[k] = ( m[k][i] + m[i][k] ) * root;
		}

		return q;
	}
}
