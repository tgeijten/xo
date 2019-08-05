#include "xo/system/test_framework.h"
#include "xo/geometry/angle.h"
#include "xo/geometry/euler_angles.h"
#include "xo/geometry/vec3.h"
#include "xo/string/string_cast.h"
#include <vector>
#include <math.h>

struct Quaternion
{
	double w, x, y, z;
};

Quaternion ToQuaternion( double yaw, double pitch, double roll ) // yaw (Z), pitch (Y), roll (X)
{
	// Abbreviations for the various angular functions
	double cy = cos( yaw * 0.5 );
	double sy = sin( yaw * 0.5 );
	double cp = cos( pitch * 0.5 );
	double sp = sin( pitch * 0.5 );
	double cr = cos( roll * 0.5 );
	double sr = sin( roll * 0.5 );

	Quaternion q;
	q.w = cy * cp * cr + sy * sp * sr;
	q.x = cy * cp * sr - sy * sp * cr;
	q.y = sy * cp * sr + cy * sp * cr;
	q.z = sy * cp * cr - cy * sp * sr;

	return q;
}

struct EulerAngles
{
	double roll, pitch, yaw;
};

EulerAngles ToEulerAngles( Quaternion q )
{
	EulerAngles angles;

	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * ( q.w * q.x + q.y * q.z );
	double cosr_cosp = +1.0 - 2.0 * ( q.x * q.x + q.y * q.y );
	angles.roll = atan2( sinr_cosp, cosr_cosp );

	// pitch (y-axis rotation)
	double sinp = +2.0 * ( q.w * q.y - q.z * q.x );
	if ( fabs( sinp ) >= 1 )
		angles.pitch = copysign( xo::constantsd::pi() / 2, sinp ); // use 90 degrees if out of range
	else
		angles.pitch = asin( sinp );

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * ( q.w * q.z + q.x * q.y );
	double cosy_cosp = +1.0 - 2.0 * ( q.y * q.y + q.z * q.z );
	angles.yaw = atan2( siny_cosp, cosy_cosp );

	return angles;
}

namespace xo
{
	XO_TEST_CASE( xo_euler_test )
	{
		auto test_euler_conversion = [&]( const vec3radd& ea, euler_order eo )
		{
			auto q = quat_from_euler( ea, eo );
			auto ea2 = euler_from_quat( q, eo );
			auto ea_val = vec3d( vec3_< degreed >( ea ) );
			auto ea2_val = vec3d( vec3_< degreed >( ea2 ) );
			XO_CHECK_MESSAGE( equal( ea_val, ea2_val ), to_str( ea_val ) + " != " + to_str( ea2_val ) );
		};

		vec3radd ea( 0.1_rad, 0.2_rad, 0.3_rad );

		test_euler_conversion( ea, euler_order::xyz );
		test_euler_conversion( ea, euler_order::xzy );
		test_euler_conversion( ea, euler_order::yxz );
		test_euler_conversion( ea, euler_order::yzx );
		test_euler_conversion( ea, euler_order::zyx );
		test_euler_conversion( ea, euler_order::zxy );

		auto q_xyz = quat_from_euler( ea, euler_order::xyz );
		auto ea_xyz = euler_xyz_from_quat( q_xyz );

		auto q_zyx = quat_from_euler( ea, euler_order::zyx );
		auto ea_zyx = euler_zyx_from_quat( q_zyx );

		auto wiki_q = ToQuaternion( 0.3, 0.2, 0.1 );
		auto wiki_angles = ToEulerAngles( wiki_q );

	}

	XO_TEST_CASE( xo_angle_decomposition_test )
	{
		auto q = quat_from_euler( 2_rad, 0_rad, 1_rad, euler_order::zyx );
		auto d = rotation_around_axis( q, vec3d::unit_x() );
		auto r = rotation_vector_from_quat( q );
		auto testd = dot_product( r, vec3d::unit_x() );
		XO_CHECK( equal( d.value, 2.0 ) );
	}
}
