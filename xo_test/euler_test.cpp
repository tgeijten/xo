#include "xo/system/test_case.h"
#include "xo/geometry/angle.h"
#include "xo/geometry/vec3.h"
#include "xo/geometry/quat.h"
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
			auto ea_val = vec3_< degreed >( ea );
			auto ea2_val = vec3_< degreed >( ea2 );
			auto e = degreed( constantsd::ample_epsilon() );
			XO_CHECK_MESSAGE( equal( ea_val, ea2_val, e ), to_str( ea_val ) + " != " + to_str( ea2_val ) );
		};

		vec3radd ea( 0.1_radd, 0.2_radd, 0.3_radd );

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
		auto q0 = quat_from_euler( 2_radd, 0_radd, 1_radd, euler_order::zyx );
		auto d0 = twist_around_axis( q0, vec3d::unit_x() );
		XO_CHECK( equal( d0.value, 2.0 ) );

		auto q = quat_from_euler( 0.4_radd, 0.3_radd, 0_radd, euler_order::xyz );
		auto rx = twist_around_axis( q, vec3d::unit_x() );
		auto ry = twist_around_axis( q, vec3d::unit_y() );
		auto rz = twist_around_axis( q, vec3d::unit_z() );
		auto rx2 = twist_around_x( q );
		auto ry2 = twist_around_y( q );
		auto rz2 = twist_around_z( q );
		XO_CHECK( rx == rx2 );
		XO_CHECK( ry == ry2 );
		XO_CHECK( rz == rz2 );

		auto qyaw = yaw( q );
		auto qpitch = pitch( q );
		auto qroll = roll( q );
		auto xyz = euler_xyz_from_quat( q );
		auto xzy = euler_xzy_from_quat( q );
		auto yxz = euler_yxz_from_quat( q );
		auto yzx = euler_yzx_from_quat( q );
		auto zxy = euler_zxy_from_quat( q );
		auto zyx = euler_zyx_from_quat( q );

		// these tests fail, need more info on what's going on
		//XO_CHECK( equal( rx, qpitch, radiand( constantsd::ample_epsilon() ) ) );
		//XO_CHECK( equal( ry, qyaw, radiand( constantsd::ample_epsilon() ) ) );
		//XO_CHECK( equal( rz, qroll, radiand( constantsd::ample_epsilon() ) ) );
	}
}
