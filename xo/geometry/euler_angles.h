#pragma once

#include "quat.h"
#include "xo/utility/smart_enum.h"
#include "xo/geometry/vec3_type.h"

namespace xo
{
	/// Euler order for Euler angle conversions
	xo_smart_enum_class( euler_order, xyz, xzy, yxz, yzx, zxy, zyx );

	template< typename T > using vec3rad_ = vec3_< radian_<T> >;
	using vec3radf = vec3rad_< float >;
	using vec3radd = vec3rad_< double >;

	template< angle_unit U, typename T > quat_<T> quat_from_x_angle( const angle_<U, T>& a )
	{
		T ha = T( 0.5 ) * a.rad_value(); return quat_<T>( std::cos( ha ), std::sin( ha ), T( 0 ), T( 0 ) );
	}

	template< angle_unit U, typename T > quat_<T> quat_from_y_angle( const angle_<U, T>& a )
	{
		T ha = T( 0.5 ) * a.rad_value(); return quat_<T>( std::cos( ha ), T( 0 ), std::sin( ha ), T( 0 ) );
	}

	template< angle_unit U, typename T > quat_<T> quat_from_z_angle( const angle_<U, T>& a )
	{
		T ha = T( 0.5 ) * a.rad_value(); return quat_<T>( std::cos( ha ), T( 0 ), T( 0 ), std::sin( ha ) );
	}

	/// make quaternion from Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( const angle_<U, T>& x, const angle_<U, T>& y, const angle_<U, T>& z, euler_order eo = euler_order::xyz ) {
		quat_<T> qx = quat_from_x_angle( x );
		quat_<T> qy = quat_from_y_angle( y );
		quat_<T> qz = quat_from_z_angle( z );

		// #todo: more efficient, enable compile-time switching
		switch ( eo )
		{
		case euler_order::xyz: return qx * qy * qz;
		case euler_order::xzy: return qx * qz * qy;
		case euler_order::yxz: return qy * qx * qz;
		case euler_order::yzx: return qy * qz * qx;
		case euler_order::zxy: return qz * qx * qy;
		case euler_order::zyx: return qz * qy * qx;
		default: xo_error( "quat_from_euler: invalid euler_order" );
		}
	}

	/// make quaternion from vec3 of Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( const vec3_< angle_<U, T> >& eu, euler_order eo = euler_order::xyz ) {
		return quat_from_euler( eu.x, eu.y, eu.z, eo );
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

	template< typename T > vec3rad_<T> make_vec3rad( const T& x, const T& y, const T& z ) {
		// #todo: this may be redundant if we have xo::atan2 / xo::asin, etc. returning radians
		return vec3rad_<T>( radian_<T>( x ), radian_<T>( y ), radian_<T>( z ) );
	}

	template< typename T > vec3rad_<T> euler_xyz_from_quat( const quat_<T>& q )
	{
		return make_vec3rad(
			std::atan2( -2 * ( q.y * q.z - q.w * q.x ), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z ),
			std::asin( 2 * ( q.x * q.z + q.w * q.y ) ),
			std::atan2( -2 * ( q.x * q.y - q.w * q.z ), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z )
		);
	}

	template< typename T > vec3rad_<T> euler_zyx_from_quat( const quat_<T>& q )
	{
		return make_vec3rad(
			std::atan2( 2 * ( q.y * q.z + q.w * q.x ), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z ),
			std::asin( -2 * ( q.x * q.z - q.w * q.y ) ),
			std::atan2( 2 * ( q.x * q.y + q.w * q.z ), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z )
		);
	}

	template< typename T > vec3rad_<T> euler_zxy_from_quat( const quat_<T>& q )
	{
		return make_vec3rad(
			std::asin( 2 * ( q.y * q.z + q.w * q.x ) ),
			std::atan2( -2 * ( q.x * q.z - q.w * q.y ),
				q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z ),
			std::atan2( -2 * ( q.x * q.y - q.w * q.z ),
				q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z )
		);
	}

	/// get Euler angles from quat
	template< typename T > vec3rad_<T> euler_from_quat( const quat_<T>& q, euler_order eo = euler_order::xyz )
	{
		switch ( eo )
		{
		case euler_order::xyz: return euler_xyz_from_quat( q );
		case euler_order::zxy: return euler_zxy_from_quat( q );
		case euler_order::zyx: return euler_zyx_from_quat( q );
		default:
			xo_error( "Unsupported euler_order for euler_from_quat():" + to_str( eo ) );
		}
	}
}
