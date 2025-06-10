#pragma once

#include "xo/geometry/quat_type.h"
#include "xo/geometry/vec3.h"
#include "xo/geometry/mat33.h"
#include "xo/geometry/angle.h"
#include "xo/system/assert.h"
#include "xo/numerical/compare.h"
#include "mat33_type.h"
#include "xo/utility/smart_enum.h"

namespace xo
{
	/// Euler order for Euler angle conversions
	xo_smart_enum_class( euler_order, xyz, xzy, yxz, yzx, zxy, zyx );

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
		uv *= T( 2 ) * q.w;
		uuv *= T( 2 );
		return v + uv + uuv;
	}

	/// Quaternion multiplication
	template< typename T > quat_<T>& operator*=( quat_<T>& q1, const quat_<T>& q2 ) { q1 = q1 * q2; return q1; }

	/// Quaternion scaling
	template< typename T > quat_<T>& operator*=( quat_<T>& q, const T& s )
	{ q.w *= s; q.x *= s; q.y *= s; q.z *= s; return q; }

	/// get length of a quat
	template< typename T > T length( const quat_<T>& q )
	{ return std::sqrt( q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z ); }

	/// get length of the imaginary (xyz) part of quat
	template< typename T > T length_xyz( const quat_<T>& q )
	{ return std::sqrt( q.x * q.x + q.y * q.y + q.z * q.z ); }

	template< typename T > T squared_length( const quat_<T>& q )
	{ return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z; }

	/// test if a quat is of unit length
	template< typename T > bool is_normalized( const quat_<T>& q )
	{ return std::abs( squared_length( q ) - T( 1 ) ) <= constants<T>::ample_epsilon(); }

	/// test if a quat is identity, based on epsilon (use this instead of equal)
	template< typename T > bool is_identity( const quat_<T>& q, T e = constants<T>::ample_epsilon() )
	{ return equal( q.w, T( 1 ), e ) && fabs( q.x ) <= e && fabs( q.y ) <= e && fabs( q.z ) <= e; }

	/// Compare quaternions
	template< typename T > bool operator==( const quat_<T>& q1, const quat_<T>& q2 )
	{ return q1.w == q2.w && q1.x == q2.x && q1.y == q2.y && q1.z == q2.z; }
	template< typename T > bool operator!=( const quat_<T>& q1, const quat_<T>& q2 )
	{ return !( q1 == q2 ); }

	/// Compare quaternions with epsilon
	template< typename T > bool equal( const quat_<T>& q1, const quat_<T>& q2, T e = constants<T>::ample_epsilon() )
	{ return equal( q1.w, q2.w, e ) && equal( q1.x, q2.x, e ) && equal( q1.y, q2.y, e ) && equal( q1.z, q2.z, e ); }

	/// normalize quaternion, return length
	template< typename T > T normalize( quat_<T>& q )
	{ T len = length( q ); T s = inv( len ); q *= s; return len; }

	/// get normalized quaternion
	template< typename T > quat_<T> normalized( quat_<T> q ) { normalize( q ); return q; }

	/// normalize quaternion, return length, handles zero quaternion
	template< typename T > T normalize_safe( quat_<T>& q ) {
		T len = length( q );
		if ( len > constants<T>::epsilon() ) q *= inv( len ); else q = quat_<T>::identity();
		return len;
	}

	/// get normalized quaternion, handles zero quaternion
	template< typename T > quat_<T> normalized_safe( quat_<T> q ) { normalize_safe( q ); return q; }

	// Fast normalization using first order Padé approximant, works only when ||q|| ~ 1
	template< typename T > void normalize_fast( quat_<T>& q )
	{ T sl = squared_length( q ); q *= T( 2 ) / ( T( 1 ) + sl ); }

	/// get normalized quaternion using first order Padé approximant, works only when ||q|| ~ 1
	template< typename T > quat_<T> normalized_fast( quat_<T> q ) { normalize_fast( q ); return q; }

	// Perform either fast or normal normalization, based on epsilon of squared length
	template< typename T > void normalize_try_fast( quat_<T>& q, const T epsilon = T( 2.107342e-08 ) ) {
		const auto slen = squared_length( q );
		if ( std::abs( T( 1 ) - slen ) < epsilon )
			q *= T( 2 ) / ( T( 1 ) + slen ); // first order Padé approximant
		else q *= T( 1 ) / std::sqrt( slen );
	}

	/// get normalized quaternion
	template< typename T > quat_<T> normalized_try_fast( quat_<T> q ) { normalize_try_fast( q ); return q; }

	/// get quaternion conjugate
	template< typename T > quat_<T> conjugate( quat_<T> q ) { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }

	/// get quaternion conjugate
	template< typename T > quat_<T> operator-( quat_<T> q ) { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }

	/// get quaternion inverse; #todo: verify correctness
	template< typename T > quat_<T> inverse( quat_<T> q )
	{ auto f = T( -1 ) / squared_length( q ); q.x *= f; q.y *= f; q.z *= f; return q; }

	/// return quaternion in which w is positive (negate if w < 0)
	template< typename T > quat_<T> positive( quat_<T> q )
	{ if ( q.w < 0 ) { q.w = -q.w; q.x = -q.x; q.y = -q.y; q.z = -q.z; } return q; }

	/// make quaternion from axis and angle
	template< angle_unit U, typename T > quat_<T> quat_from_axis_angle( const vec3_<T>& axis, angle_<U, T> ang ) {
		xo_debug_assert( is_normalized<T>( axis ) );
		T ha = T( 0.5 ) * ang.rad_value();
		T hs = std::sin( ha );
		return quat_<T>( std::cos( ha ), hs * axis.x, hs * axis.y, hs * axis.z );
	}

	/// make quaternion from axis and angle
	template< typename T > quat_<T> quat_from_rotation_vector( vec3_<T> v ) {
		auto l = length( v );
		if ( l > constants<T>::epsilon() ) {
			v /= l;
			T ha = T( 0.5 ) * l;
			T hs = std::sin( ha );
			return quat_<T>( std::cos( ha ), hs * v.x, hs * v.y, hs * v.z );
		}
		else return quat_<T>::identity();
	}

	/// Get quaternion to represent the rotation from source to target vector
	template< typename T > quat_<T> quat_from_directions( const vec3_<T>& source, const vec3_<T>& target ) {
		vec3_<T> axis = cross_product( source, target );
		T dot = dot_product( source, target );
		if ( dot < -1.0f + constants<T>::ample_epsilon() )
			return quat_<T>( 0, 1, 0, 0 );
		quat_<T> result( dot + 1.0f, axis.x, axis.y, axis.z );
		normalize( result );
		return result;
	}

	/// Get quaternion to represent the rotation from source to target quaternion
	template< typename T > quat_<T> quat_from_quats( const quat_<T>& source, const quat_<T>& target )
	{ return -source * target; }

	/// Get rotation vector from normalized quaternion
	template< typename T > vec3_<T> rotation_vector_from_quat( const quat_<T>& q ) {
		T l = std::sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
		if ( ( l > constants<T>::ample_epsilon() ) & ( q.w < T( 1 ) ) ) {
			T f = T( 2 ) * std::acos( q.w ) / l;
			return vec3_<T>( f * q.x, f * q.y, f * q.z );
		}
		else return vec3_<T>::zero();
	}

	/// Get rotation vector from normalized quaternion
	template< typename T > std::pair< vec3_<T>, radian_<T> > rotation_vector_angle_from_quat( const quat_<T>& q ) {
		T l = std::sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
		if ( ( l > constants<T>::ample_epsilon() ) & ( q.w < T( 1 ) ) ) {
			T a = T( 2 ) * std::acos( q.w );
			T s = a / l;
			return { vec3_<T>( s * q.x, s * q.y, s * q.z ), radian_<T>( a ) };
		}
		else return { vec3_<T>::zero(), radian_<T>( 0 ) };
	}

	/// Get axis angle from normalized quaternion
	template< typename T > std::pair< vec3_<T>, radian_<T> > axis_angle_from_quat( const quat_<T>& q ) {
		T l = std::sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
		if ( ( l > constants<T>::ample_epsilon() ) & ( q.w < T( 1 ) ) ) {
			T s = T( 1 ) / l;
			return { vec3_<T>( s * q.x, s * q.y, s * q.z ), radian_<T>( T( 2 ) * std::acos( q.w ) ) };
		}
		else return { vec3_<T>::unit_x(), radian_<T>() };
	}

	/// Get the twist quaternion and angle around specific unit-length axis
	template< typename T > std::pair< quat_<T>, radian_<T> > quat_twist_around_axis( const quat_<T>& q, const vec3_<T>& a ) {
		auto d = dot_product( vec3_<T>( q.x, q.y, q.z ), a );
		auto qa = quat_<T>( q.w, d * a.x, d * a.y, d * a.z ); // assume ||a|| == 1
		if ( auto l = length( qa ); l > constants<T>::epsilon() )
			qa *= inv( l );
		else qa.w = 1; // is this the best we can do?
		return { qa, radian_<T>( std::copysign( 2 * std::acos( qa.w ), d ) ) };
	}

	/// Get twist around specific unit-length axis
	template< typename T > radian_<T> twist_around_axis( const quat_<T>& q, const vec3_<T>& a ) {
		auto d = dot_product( vec3_<T>( q.x, q.y, q.z ), a );
		auto qa = quat_<T>( q.w, d * a.x, d * a.y, d * a.z ); // assume ||a|| == 1
		if ( auto l = length( qa ); l > constants<T>::epsilon() )
			return radian_<T>( std::copysign( 2 * std::acos( qa.w / l ), d ) );
		else return radian_<T>( 0 );
	}

	/// Get twist quaternion and angle around x-axis
	template< typename T > std::pair< quat_<T>, radian_<T> > quat_twist_around_x( const quat_<T>& q ) {
		if ( auto l = std::sqrt( q.w * q.w + q.x * q.x ); l > constants<T>::epsilon() ) {
			auto s = inv( l ); auto w = s * q.w;
			return { quat_<T>( w, s * q.x, T( 0 ), T( 0 ) ), radian_<T>( std::copysign( 2 * std::acos( w ), q.x ) ) };
		} else return { {}, {} };
	}

	/// Get twist quaternion and angle around y-axis
	template< typename T > std::pair< quat_<T>, radian_<T> > quat_twist_around_y( const quat_<T>& q ) {
		if ( auto l = std::sqrt( q.w * q.w + q.y * q.y ); l > constants<T>::epsilon() ) {
			auto s = inv( l ); auto w = s * q.w;
			return { quat_<T>( w, T( 0 ), s * q.y, T( 0 ) ), radian_<T>( std::copysign( 2 * std::acos( w ), q.y ) ) };
		} else return { {}, {} };
	}

	/// Get twist quaternion and angle around z-axis
	template< typename T > std::pair< quat_<T>, radian_<T> > quat_twist_around_z( const quat_<T>& q ) {
		if ( auto l = std::sqrt( q.w * q.w + q.z * q.z ); l > constants<T>::epsilon() ) {
			auto s = inv( l ); auto w = s * q.w;
			return { quat_<T>( w, T( 0 ), T( 0 ), s * q.z ), radian_<T>( std::copysign( 2 * std::acos( w ), q.z ) ) };
		} else return { {}, {} };
	}

	/// Twist around local x axis
	// #todo: handle singularity when q.w == 0 && q.x == 0
	template< typename T > radian_<T> twist_around_x( const quat_<T>& q ) {
		return radian_<T>( std::copysign( 2 * std::acos( q.w / std::sqrt( q.w * q.w + q.x * q.x ) ), q.x ) );
	}

	/// Twist around local y axis
	// #todo: handle singularity when q.w == 0 && q.y == 0
	template< typename T > radian_<T> twist_around_y( const quat_<T>& q ) {
		return radian_<T>( std::copysign( 2 * std::acos( q.w / std::sqrt( q.w * q.w + q.y * q.y ) ), q.y ) );
	}

	/// Twist around local z axis
	// #todo: handle singularity when q.w == 0 && q.z == 0
	template< typename T > radian_<T> twist_around_z( const quat_<T>& q ) {
		return radian_<T>( std::copysign( 2 * std::acos( q.w / std::sqrt( q.w * q.w + q.z * q.z ) ), q.z ) );
	}

	/// Get angle around specific unit-length axis
	template< typename T > radian_<T> angle_around_axis( const quat_<T>& q, const vec3_<T>& a ) {
		auto dot_vec = q.x * a.x + q.y * a.y + q.z * a.z;
		auto theta = 2 * std::acos( q.w );
		auto sin_half_theta = std::sqrt( T( 1 ) - q.w * q.w );
		if ( sin_half_theta > xo::num<T>::epsilon )
			return radian_<T>( theta * dot_vec / sin_half_theta );
		else return radian_<T>( T( 2 ) * dot_vec );
	}

	/// Get quaternion and angle around x-axis
	template< typename T > std::pair< quat_<T>, radian_<T> > quat_angle_around_x( const quat_<T>& q ) {
		T a = 2 * std::acos( q.w ), sin_half_a = std::sqrt( 1 - q.w * q.w );
		T ax = sin_half_a > xo::num<T>::epsilon ? a * q.x / sin_half_a : 2 * q.x;
		T half_ax = T( 0.5 ) * ax;
		return { quat_<T>( std::cos( half_ax ), std::sin( half_ax ), 0, 0 ), radian_<T>( ax ) };
	}

	/// Get quaternion and angle around y-axis
	template< typename T > std::pair< quat_<T>, radian_<T> > quat_angle_around_y( const quat_<T>& q ) {
		T a = 2 * std::acos( q.w ), sin_half_a = std::sqrt( 1 - q.w * q.w );
		T ay = sin_half_a > xo::num<T>::epsilon ? a * q.y / sin_half_a : 2 * q.y;
		T half_ay = T( 0.5 ) * ay;
		return { quat_<T>( std::cos( half_ay ), 0, std::sin( half_ay ), 0 ), radian_<T>( ay ) };
	}

	/// Get quaternion and angle around z-axis
	template< typename T > std::pair< quat_<T>, radian_<T> > quat_angle_around_z( const quat_<T>& q ) {
		T a = 2 * std::acos( q.w ), sin_half_a = std::sqrt( 1 - q.w * q.w );
		T az = sin_half_a > xo::num<T>::epsilon ? a * q.z / sin_half_a : 2 * q.z;
		T half_az = T( 0.5 ) * az;
		return { quat_<T>( std::cos( half_az ), 0, 0, std::sin( half_az ) ), radian_<T>( az ) };
	}

	template< typename T > vec3_<T> local_x_axis( const quat_<T>& q ) {
		T tx = q.x + q.x, ty = q.y + q.y, tz = q.z + q.z;
		T twy = ty * q.w, twz = tz * q.w;
		T txy = ty * q.x, txz = tz * q.x;
		T tyy = ty * q.y, tzz = tz * q.z;
		return vec3_<T>( 1.0f - ( tyy + tzz ), txy + twz, txz - twy );
	}

	template< typename T > vec3_<T> local_y_axis( const quat_<T>& q ) {
		T tx = q.x + q.x, ty = q.y + q.y, tz = q.z + q.z;
		T twx = tx * q.w, twz = tz * q.w;
		T txx = tx * q.x, txy = ty * q.x;
		T tyz = tz * q.y, tzz = tz * q.z;
		return vec3_<T>( txy - twz, 1.0f - ( txx + tzz ), tyz + twx );
	}

	template< typename T > vec3_<T> local_z_axis( const quat_<T>& q ) {
		T tx = q.x + q.x, ty = q.y + q.y, tz = q.z + q.z;
		T twx = tx * q.w, twy = ty * q.w;
		T txx = tx * q.x, txz = tz * q.x;
		T tyy = ty * q.y, tyz = tz * q.y;
		return vec3_<T>( txz + twy, tyz - twx, 1.0f - ( txx + tyy ) );
	}

	/// Get axes from quaternion
	template< typename T > vec3_< vec3_<T> > axes_from_quat( const quat_<T>& q ) {
		T tx = q.x + q.x, ty = q.y + q.y, tz = q.z + q.z;
		T twx = tx * q.w, twy = ty * q.w, twz = tz * q.w;
		T txx = tx * q.x, txy = ty * q.x, txz = tz * q.x;
		T tyy = ty * q.y, tyz = tz * q.y, tzz = tz * q.z;
		return {
			{ 1.0f - ( tyy + tzz ), txy + twz, txz - twy },
			{ txy - twz, 1.0f - ( txx + tzz ), tyz + twx },
			{ txz + twy, tyz - twx, 1.0f - ( txx + tyy ) } };
	}

	template< angle_unit U, typename T > quat_<T> quat_from_x_angle( const angle_<U, T>& a ) {
		T ha = T( 0.5 ) * a.rad_value();
		return quat_<T>( std::cos( ha ), std::sin( ha ), T( 0 ), T( 0 ) );
	}

	template< angle_unit U, typename T > quat_<T> quat_from_y_angle( const angle_<U, T>& a ) {
		T ha = T( 0.5 ) * a.rad_value();
		return quat_<T>( std::cos( ha ), T( 0 ), std::sin( ha ), T( 0 ) );
	}

	template< angle_unit U, typename T > quat_<T> quat_from_z_angle( const angle_<U, T>& a ) {
		T ha = T( 0.5 ) * a.rad_value();
		return quat_<T>( std::cos( ha ), T( 0 ), T( 0 ), std::sin( ha ) );
	}

	/// quaternion from Euler angles
	// #perf: can be done more efficient
	template< angle_unit U, typename T > quat_<T> quat_from_euler_xyz( const vec3_< angle_<U, T> >& eu ) {
		return quat_from_x_angle( eu.x ) * quat_from_y_angle( eu.y ) * quat_from_z_angle( eu.z );
	}
	template< angle_unit U, typename T > quat_<T> quat_from_euler_xzy( const vec3_< angle_<U, T> >& eu ) {
		return quat_from_x_angle( eu.x ) * quat_from_z_angle( eu.z ) * quat_from_y_angle( eu.y );
	}
	template< angle_unit U, typename T > quat_<T> quat_from_euler_yxz( const vec3_< angle_<U, T> >& eu ) {
		return quat_from_y_angle( eu.y ) * quat_from_x_angle( eu.x ) * quat_from_z_angle( eu.z );
	}
	template< angle_unit U, typename T > quat_<T> quat_from_euler_yzx( const vec3_< angle_<U, T> >& eu ) {
		return quat_from_y_angle( eu.y ) * quat_from_z_angle( eu.z ) * quat_from_x_angle( eu.x );
	}
	template< angle_unit U, typename T > quat_<T> quat_from_euler_zxy( const vec3_< angle_<U, T> >& eu ) {
		return quat_from_z_angle( eu.z ) * quat_from_x_angle( eu.x ) * quat_from_y_angle( eu.y );
	}
	template< angle_unit U, typename T > quat_<T> quat_from_euler_zyx( const vec3_< angle_<U, T> >& eu ) {
		return quat_from_z_angle( eu.z ) * quat_from_y_angle( eu.y ) * quat_from_x_angle( eu.x );
	}

	/// quaternion from Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( const vec3_< angle_<U, T> >& eu, euler_order eo = euler_order::xyz ) {
		switch ( eo ) {
		case euler_order::xyz: return quat_from_euler_xyz( eu );
		case euler_order::xzy: return quat_from_euler_xzy( eu );
		case euler_order::yxz: return quat_from_euler_yxz( eu );
		case euler_order::yzx: return quat_from_euler_yzx( eu );
		case euler_order::zxy: return quat_from_euler_zxy( eu );
		case euler_order::zyx: return quat_from_euler_zyx( eu );
		default: xo_error( "quat_from_euler: invalid euler_order" );
		}
	}

	template< angle_unit U, typename T > quat_<T> quat_from_euler( const angle_<U, T>& x, const angle_<U, T>& y, const angle_<U, T>& z, euler_order eo = euler_order::xyz ) {
		return quat_from_euler( vec3_< angle_<U, T> >( x, y, z ), eo );
	}

	// #todo: verify / replace
	template< typename T > radian_< T > pitch( const quat_<T>& q ) {
		T tx = T( 2 ) * q.x, tz = T( 2 ) * q.z;
		T twx = tx * q.w, txx = tx * q.x, tyz = tz * q.y, tzz = tz * q.z;
		return radian_< T >( atan2( tyz + twx, T( 1 ) - ( txx + tzz ) ) );
	}

	// #todo: verify / replace
	template< typename T > radian_< T > yaw( const quat_<T>& q ) {
		T tx = T( 2 ) * q.x, ty = T( 2 ) * q.y, tz = T( 2 ) * q.z;
		T twy = ty * q.w, txx = tx * q.x, txz = tz * q.x, tyy = ty * q.y;
		return radian_< T >( atan2( txz + twy, T( 1 ) - ( txx + tyy ) ) );
	}

	// #todo: verify / replace
	template< typename T > radian_< T > roll( const quat_<T>& q ) {
		T ty = T( 2 ) * q.y, tz = T( 2 ) * q.z;
		T twz = tz * q.w, txy = ty * q.x, tyy = ty * q.y, tzz = tz * q.z;
		return radian_< T >( atan2( txy + twz, T( 1 ) - ( tyy + tzz ) ) );
	}

	template< typename T > vec3rad_<T> euler_xyz_from_quat( const quat_<T>& q )
	{
		return vec3rad_<T>(
			std::atan2( -2 * ( q.y * q.z - q.w * q.x ), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z ),
			std::asin( 2 * ( q.x * q.z + q.w * q.y ) ),
			std::atan2( -2 * ( q.x * q.y - q.w * q.z ), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z )
			);
	}

	template< typename T > vec3rad_<T> euler_xzy_from_quat( const quat_<T>& q )
	{
		return vec3rad_<T>(
			std::atan2( 2 * ( q.y * q.z + q.w * q.x ), q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z ),
			std::atan2( 2 * ( q.x * q.z + q.w * q.y ), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z ),
			std::asin( -2 * ( q.x * q.y - q.w * q.z ) )
			);
	}

	template< typename T > vec3rad_<T> euler_yxz_from_quat( const quat_<T>& q )
	{
		return vec3rad_<T>(
			std::asin( -2 * ( q.y * q.z - q.w * q.x ) ),
			std::atan2( 2 * ( q.x * q.z + q.w * q.y ), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z ),
			std::atan2( 2 * ( q.x * q.y + q.w * q.z ), q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z )
			);
	}

	template< typename T > vec3rad_<T> euler_yzx_from_quat( const quat_<T>& q )
	{
		return vec3rad_<T>(
			std::atan2( -2 * ( q.y * q.z - q.w * q.x ), q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z ),
			std::atan2( -2 * ( q.x * q.z - q.w * q.y ), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z ),
			std::asin( 2 * ( q.x * q.y + q.w * q.z ) )
			);
	}

	template< typename T > vec3rad_<T> euler_zxy_from_quat( const quat_<T>& q )
	{
		return vec3rad_<T>(
			std::asin( 2 * ( q.y * q.z + q.w * q.x ) ),
			std::atan2( -2 * ( q.x * q.z - q.w * q.y ), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z ),
			std::atan2( -2 * ( q.x * q.y - q.w * q.z ), q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z )
			);
	}

	template< typename T > vec3rad_<T> euler_zyx_from_quat( const quat_<T>& q )
	{
		return vec3rad_<T>(
			std::atan2( 2 * ( q.y * q.z + q.w * q.x ), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z ),
			std::asin( -2 * ( q.x * q.z - q.w * q.y ) ),
			std::atan2( 2 * ( q.x * q.y + q.w * q.z ), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z )
			);
	}

	/// get Euler angles from quat
	template< typename T > vec3rad_<T> euler_from_quat( const quat_<T>& q, euler_order eo = euler_order::xyz )
	{
		switch ( eo )
		{
		case euler_order::xyz: return euler_xyz_from_quat( q );
		case euler_order::xzy: return euler_xzy_from_quat( q );
		case euler_order::yxz: return euler_yxz_from_quat( q );
		case euler_order::yzx: return euler_yzx_from_quat( q );
		case euler_order::zxy: return euler_zxy_from_quat( q );
		case euler_order::zyx: return euler_zyx_from_quat( q );
		default:
			xo_error( "Unsupported euler_order for euler_from_quat():" + to_str( eo ) );
		}
	}

	/// convert quat to string
	template< typename T > string to_str( const quat_<T>& q ) {
		return to_str( q.w ) + ' ' + to_str( q.x ) + ' ' + to_str( q.y ) + ' ' + to_str( q.z );
	}

	/// convert prop_node to quat
	template< typename T > bool from_prop_node( const prop_node& pn, quat_<T>& q ) {
		if ( pn.size() == 4 && pn.has_key( "w" ) ) {
			// quaternion with w, x, y, z components
			q.set( pn.get<T>( "w" ), pn.get<T>( "x" ), pn.get<T>( "y" ), pn.get<T>( "z" ) );
			return true;
		}
		else if ( vec3_<T> v; from_prop_node( pn, v ) ) {
			// quaternion from Euler angles
			auto in_radians = pn.get( "in_radians", !pn.get( "in_degrees", true ) );
			auto ea = in_radians ? vec3_<radian_<T>>( v ) : vec3_<radian_<T>>( vec3_<degree_<T>>( v ) );
			auto order = pn.get< euler_order >( "order", euler_order::xyz );
			q = quat_from_euler( ea, order );
			return true;
		}
		else return false;
	};


	/// convert quat to prop_node
	template< typename T > prop_node to_prop_node( const quat_<T>& q ) {
		prop_node pn;
		pn.set( "w", q.w );
		pn.set( "x", q.x );
		pn.set( "y", q.y );
		pn.set( "z", q.z );
		return pn;
	}
}
