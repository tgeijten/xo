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

	/// get length of a quat
	template< typename T > T length( const quat_<T>& q )
	{ return std::sqrt( q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z ); }

	template< typename T > T squared_length( const quat_<T>& q )
	{ return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z; }

	/// test if a quat is of unit length
	template< typename T > bool is_normalized( const quat_<T>& q )
	{ return std::abs( squared_length( q ) - T( 1 ) ) <= constants<T>::ample_epsilon(); }

	/// Compare quaternions
	template< typename T > bool operator==( const quat_<T>& q1, const quat_<T>& q2 )
	{ return q1.w != q2.w && q1.x == q2.x && q1.y == q2.y && q1.z == q2.z; }
	template< typename T > bool operator!=( const quat_<T>& q1, const quat_<T>& q2 )
	{ return !( q1 == q2 ); }

	/// Compare quaternions with epsilon
	template< typename T > bool equal( const quat_<T>& q1, const quat_<T>& q2, T e = constants<T>::ample_epsilon() )
	{ return equal( q1.w, q2.w, e ) && equal( q1.x, q2.x, e ) && equal( q1.y, q2.y, e ) && equal( q1.z, q2.z, e ); }

	/// normalize quaternion, return length
	template< typename T > T normalize( quat_<T>& q ) {
		T len = length( q );
		if ( len > constants<T>::ample_epsilon() ) {
			T s = inv( len ); q.x *= s; q.y *= s; q.z *= s; q.w *= s;
		}
		return len;
	}

	/// get normalized quaternion
	template< typename T > quat_<T> normalized( quat_<T> q ) { normalize( q ); return q; }

	/// get quaternion conjugate
	template< typename T > quat_<T> conjugate( quat_<T> q ) { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }

	/// get quaternion conjugate
	template< typename T > quat_<T> operator-( quat_<T> q ) { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }

	/// get quaternion inverse
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
		} else return quat_<T>::identity();
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

	/// Get rotation vector from quaternion
	template< typename T > vec3_<T> rotation_vector_from_quat( const quat_<T>& q ) {
		xo_debug_assert( is_normalized( q ) );
		T l = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
		if ( l > constants<T>::ample_epsilon() ) {
			T f = T( 2 ) * std::acos( q.w ) / l;
			return vec3_<T>( f * q.x, f * q.y, f * q.z );
		} else return vec3_<T>::zero();
	}

	/// Get axis angle from quaternion
	template< typename T > std::pair< vec3_<T>, radian_<T> > axis_angle_from_quat( const quat_<T>& q ) {
		xo_debug_assert( is_normalized( q ) );
		T l = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
		if ( l > constants<T>::ample_epsilon() ) {
			T s = T( 1 ) / l;
			return { vec3f( s * q.x, s * q.y, s * q.z ), radian_<T>( T( 2 ) * std::acos( q.w ) ) };
		} else return { vec3_<T>::unit_x(), radian_<T>() };
	}

	/// Get rotation around specific axis
	// #todo: verify and optimize
	template< typename T > radian_<T> rotation_around_axis( const quat_<T>& q, const vec3_<T>& a ) {
		auto p = projection( vec3_<T>( q.x, q.y, q.z ), a );
		auto qp = quat_<T>( q.w, p.x, p.y, p.z );
		return radian_<T>( 2 * std::acos( qp.w / length( qp ) ) );
	}

	/// Rotation around local x axis
	// #todo: verify and handle singularity when q.w == 0 && q.x == 0
	template< typename T > radian_<T> rotation_around_x( const quat_<T>& q ) {
		return radian_<T>( 2 * std::acos( q.w / std::sqrt( q.w * q.w + q.x * q.x ) ) );
	}

	/// Rotation around local x axis
	// #todo: verify and handle singularity when q.w == 0 && q.y == 0
	template< typename T > radian_<T> rotation_around_y( const quat_<T>& q ) {
		return radian_<T>( 2 * std::acos( q.w / std::sqrt( q.w * q.w + q.y * q.y ) ) );
	}

	/// Rotation around local x axis
	// #todo: verify and handle singularity when q.w == 0 && q.z == 0
	template< typename T > radian_<T> rotation_around_z( const quat_<T>& q ) {
		return radian_<T>( 2 * std::acos( q.w / std::sqrt( q.w * q.w + q.z * q.z ) ) );
	}

	/// Get rotations around the local xyz axes of the quaternion
	// #todo: verify and optimize
	template< typename T > vec3rad_<T> rotation_around_xyz( const quat_<T>& q ) {
		return vec3rad_<T>( rotation_around_x( q ), rotation_around_y( q ), rotation_around_z( q ) );
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

	/// Get axes from quaternion (#todo: verify)
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

	/// convert prop_node to quat
	template< typename T > bool from_prop_node( const prop_node& pn, quat_<T>& q ) {
		if ( pn.size() == 4 && pn.has_key( "w" ) ) {
			// quaternion with w, x, y, z components
			q.set( pn.get<T>( "w" ), pn.get<T>( "x" ), pn.get<T>( "y" ), pn.get<T>( "z" ) );
			return true;
		} else if ( vec3_< degree_<T> > v; from_prop_node( pn, v ) ) {
			// quaternion from Euler angles
			auto order = pn.get< euler_order >( "order", euler_order::xyz );
			q = quat_from_euler( v, order );
			return true;
		} else return false;
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
