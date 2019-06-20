#pragma once

#include "quat_type.h"
#include "xo/utility/smart_enum.h"

namespace xo
{
	/// Euler order for Euler angle conversions
	xo_smart_enum_class( euler_order, xyz, xzy, yxz, yzx, zxy, zyx );

	template< typename T > using euler_angles = vec3_< radian_< T > >;

	/// make quaternion from Euler angles
	template< angle_unit U, typename T > quat_<T> quat_from_euler( angle_<U, T> x, angle_<U, T> y, angle_<U, T> z, euler_order eo = euler_order::xyz ) {
		T hxa = T( 0.5 ) * x.rad_value();
		T hya = T( 0.5 ) * y.rad_value();
		T hza = T( 0.5 ) * z.rad_value();
		quat_<T> qx = quat_<T>( std::cos( hxa ), std::sin( hxa ), T( 0 ), T( 0 ) );
		quat_<T> qy = quat_<T>( std::cos( hya ), T( 0 ), std::sin( hya ), T( 0 ) );
		quat_<T> qz = quat_<T>( std::cos( hza ), T( 0 ), T( 0 ), std::sin( hza ) );

		// #todo: more efficient
		switch ( eo )
		{
		case xo::euler_order::xyz: return qx * qy * qz;
		case xo::euler_order::xzy: return qx * qz * qy;
		case xo::euler_order::yxz: return qy * qx * qz;
		case xo::euler_order::yzx: return qy * qz * qx;
		case xo::euler_order::zxy: return qz * qx * qy;
		case xo::euler_order::zyx: return qz * qy * qx;
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

	/// get Euler angles from quat
	// #todo: add Euler order as argument, use if constexpr
	template< typename T > euler_angles< T > euler_from_quat( const quat_<T>& q )
	{
		// #todo: verify / replace
		return euler_angles< T >( pitch( q ), yaw( q ), roll( q ) );
	}

}
