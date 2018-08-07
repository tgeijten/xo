#pragma once

#include "xo/geometry/quat_type.h"
#include "xo/geometry/vec3.h"
#include "xo/geometry/mat33.h"
#include "xo/geometry/angle.h"
#include "xo/system/assert.h"
#include <ostream>

namespace xo
{
	/// Euler order for Euler angle conversions
	enum class euler_order { xyz, xzy, yxz, yzx, zxy, zyx };

	/// Quaternion multiplication
	template< typename T > quat_<T> operator*( const quat_<T>& q1, const quat_<T>& q2 ) {
		return quat_<T>(
			q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
			q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
			q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
			q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x );
	}

	/// Vector multiplication
	template< typename T > vec3_<T> operator*( const quat_<T>& q, const vec3_<T>& v ) {
		vec3_<T> qv( q.x, q.y, q.z );
		vec3_<T> uv = cross_product( qv, v );
		vec3_<T> uuv = cross_product( qv, uv );
		uv *= T(2) * q.w;
		uuv *= T(2);
		return v + uv + uuv;
	}

	/// Quaternion multiplication
	template< typename T > quat_<T>& operator*=( quat_<T>& q1, const quat_<T>& q2 ) { q1 = q1 * q2; return q1; }

	/// get length of a quat
	template< typename T > T length( quat_<T> q )
	{ return sqrt( q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z ); }

	template< typename T > T squared_length( quat_<T> q )
	{ return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z; }

	/// test if a quat is of unit length
	template< typename T > bool is_normalized( quat_<T> q )
	{ return equal( q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z, T(1) ); }

	/// normalize quaternion, return length
	template< typename T > T normalize( quat_<T>& q ) {
		T l = length( q );
		if ( l > const_epsilon<T>() )
		{ T invl = inv( l ); q.x *= invl; q.y *= invl; q.z *= invl; q.w *= invl; }
		return l;
	}

	/// get normalized quaternion
	template< typename T > quat_<T> normalized( quat_<T> q ) { normalize( q ); return q; }

	/// get quaternion conjugate
	template< typename T > quat_<T> conjugate( quat_<T> q ) { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }

	/// get quaternion inverse
	template< typename T > quat_<T> inverse( quat_<T> q )
	{ auto s = squared_length( q ); xo_assert( s > T( 0 ) ); auto f = -inv( s ); q.x *= f; q.y *= f; q.z *= f; return q; }

	/// return quaternion in which w is positive (negate if w < 0)
	template< typename T > quat_<T> positive( quat_<T> q )
	{ if ( q.w < 0 ) { q.w = -q.w; q.x = -q.x; q.y = -q.y; q.z = -q.z; } return q; }

	/// make quaternion from axis and angle
	template< angle_unit U, typename T > quat_<T> quat_from_axis_angle( const vec3_<T>& axis, angle_<U, T> ang ) {
		xo_assert( is_normalized<T>( axis ) );
		T ha = T( 0.5 ) * ang.rad_value();
		T hs = std::sin( ha );
		return quat_<T>( std::cos( ha ), hs * axis.x, hs * axis.y, hs * axis.z );
	}

	/// make quaternion from axis and angle
	template< typename T > quat_<T> quat_from_rotation_vector( vec3_<T> v ) {
		auto l = v.length();
		if ( l > const_epsilon<T>() ) {
			v /= l;
			T ha = T( 0.5 ) * l;
			T hs = std::sin( ha );
			return quat_<T>( std::cos( ha ), hs * v.x, hs * v.y, hs * v.z );
		}
		else return quat_<T>::identity();
	}

	/// make quaternion from Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( angle_<U, T> x, angle_<U, T> y, angle_<U, T> z, euler_order eo = euler_order::xyz ) {
		T hxa = T( 0.5 ) * x.rad_value(), hya = T( 0.5 ) * y.rad_value(), hza = T( 0.5 ) * z.rad_value();
		quat_<T> qx = quat_<T>( std::cos( hxa ), std::sin( hxa ), T( 0 ), T( 0 ) );
		quat_<T> qy = quat_<T>( std::cos( hya ), T( 0 ), std::sin( hya ), T( 0 ) );
		quat_<T> qz = quat_<T>( std::cos( hza ), T( 0 ), T( 0 ), std::sin( hza ) );
		// #TODO: more efficient
		switch ( eo )
		{
		case xo::euler_order::xyz: return qx * ( qy * qz );
		case xo::euler_order::xzy: return qx * ( qz * qy );
		case xo::euler_order::yxz: return qy * ( qx * qz );
		case xo::euler_order::yzx: return qy * ( qz * qx );
		case xo::euler_order::zxy: return qz * ( qx * qy );
		case xo::euler_order::zyx: return qz * ( qy * qx );
		default: xo_error( "Unsupported euler_order" );
		}
	}

	/// make quaternion from vec3 of Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( const vec3_< angle_<U, T> >& eu, euler_order eo = euler_order::xyz ) {
		return quat_from_euler( eu.x, eu.y, eu.z, eo );
	}

	/// get Euler angles from quat
	// TODO: add Euler order as argument
	template< typename T > vec3_< radian_< T > > euler_from_quat( const quat_<T>& q )
	{ return vec3_< radian_< T > >( pitch( q ), yaw( q ), roll( q ) ); }

	template< typename T > radian_< T > pitch( const quat_<T>& q ) {
		T tx = T(2) * q.x, tz = T(2) * q.z;
		T twx = tx * q.w, txx = tx * q.x, tyz = tz * q.y, tzz = tz * q.z;
		return radian_< T >( atan2( tyz + twx, T(1) - ( txx + tzz ) ) );
	}

	template< typename T > radian_< T > yaw( const quat_<T>& q ) {
		T tx = T(2) * q.x, ty = T(2) * q.y, tz = T(2) * q.z;
		T twy = ty * q.w, txx = tx * q.x, txz = tz * q.x, tyy = ty * q.y;
		return radian_< T >( atan2( txz + twy, T(1) - ( txx + tyy ) ) );
	}

	template< typename T > radian_< T > roll( const quat_<T>& q ) {
		T ty = T(2) * q.y, tz = T(2) * q.z;
		T twz = tz * q.w, txy = ty * q.x, tyy = ty * q.y, tzz = tz * q.z;
		return radian_< T >( atan2( txy + twz, T(1) - ( tyy + tzz ) ) );
	}

	/// Get quaternion to represent the rotation from source to target vector
	template< typename T > quat_<T> quat_from_directions( const vec3_<T>& source, const vec3_<T>& target ) {
		vec3_<T> s = normalized( source );
		vec3_<T> t = normalized( target );
		vec3_<T> c = cross_product( s, t );
		T d = dot_product( s, t );

		if ( equal( d, T(1) ) ) // check if vectors are the same
			return quat_<T>::identity();

		auto clen = length( c );
		if ( equal( clen, T() ) ) // check if vectors are 180 deg apart
			return quat_<T>( 0, 1, 0, 0 ); // this doesn't work if source is unit_x

		c /= clen;
		auto a = std::acos( d ) * T(0.5);
		auto sa = std::sin( a );

		return quat_<T>( std::cos( a ), c.x * sa, c.y * sa, c.z * sa );
	}

	/// Get quaternion to represent the rotation from source to target quaternion
	template< typename T > quat_<T> quat_from_quats( const quat_<T>& source, const quat_<T>& target )
	{ return -source * target; }

	/// Get rotation vector from quaternion
	template< typename T > vec3_<T> rotation_vector_from_quat( const quat_<T>& q ) {
		T l = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
		if ( l > const_epsilon<T>() ) {
			T f = T(2) * std::acos( q.w ) / l;
			return vec3_<T>( f * q.x, f * q.y, f * q.z );
		}
		else return vec3_<T>::zero();
	}

	/// Get axis angle from quaternion
	template< typename T > std::pair< vec3_<T>, radian_<T> > axis_angle_from_quat( const quat_<T>& q ) {
		//xo_assert( is_normalized( q ) );
		T l = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
		if ( l > const_epsilon<T>() ) {
			T s = T(1) / l;
			return std::pair< vec3_<T>, radian_<T> >{ vec3f( s * q.x, s * q.y, s * q.z ), radian_<T>( T(2) * std::acos( q.w ) ) };
		}
		else return std::pair< vec3_<T>, radian_<T> >{ vec3_<T>::unit_x(), radian_<T>( T() ) };
	}

	/// Get quaternion using three axis vectors
	template< typename T > quat_<T> quat_from_axes( const vec3_<T>& x, const vec3_<T>& y, const vec3_<T>& z ) {
		//xo_assert( is_normalized( x ) && is_normalized( y ) && is_normalized( z ) );
		quat_<T> q;
		mat33_<T> m = mat33_from_axes( x, y, z );

		T t = m.e00 + m.e11 + m.e22;
		if ( t > 0.0 ) {
			T root = sqrt( t + T( 1 ) );
			q.w = T( 0.5 ) * root;
			root = T( 0.5 ) / root;
			q.x = ( m.e21 - m.e12 ) * root;
			q.y = ( m[0][2] - m[2][0] ) * root;
			q.z = ( m[1][0] - m[0][1] ) * root;
		}
		else {
			size_t i = 0;
			if ( m[1][1] > m[0][0] ) i = 1;
			if ( m[2][2] > m[i][i] ) i = 2;
			size_t j = ( i + 1 ) % 3;
			size_t k = ( j + 1 ) % 3;
			T root = sqrt( m[i][i] - m[j][j] - m[k][k] + T( 1 ) );
			T* q_xyz = &q.x;
			q_xyz[i] = T( 0.5 ) * root;
			root = T( 0.5 ) / root;
			q.w = ( m[k][j] - m[j][k] ) * root;
			q_xyz[j] = ( m[j][i] + m[i][j] ) * root;
			q_xyz[k] = ( m[k][i] + m[i][k] ) * root;
		}

		return q;
	}

	/// Stream output
	template< typename T > std::ostream& operator<<( std::ostream& str, const quat_<T>& v )
	{ str << v.w << ' ' << v.x << ' ' << v.y << ' ' << v.z; return str; }
}
