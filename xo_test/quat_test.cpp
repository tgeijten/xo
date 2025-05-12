#include "xo/system/test_case.h"
#include "xo/geometry/quat.h"
#include "xo/geometry/vec3.h"
#include "xo/numerical/random.h"
#include "test_tools.h"
#include "xo/system/log.h"

namespace xo
{
	template< typename T > quat_<T> quat_from_directions_old( const vec3_<T>& source, const vec3_<T>& target ) {
		vec3_<T> s = normalized( source );
		vec3_<T> t = normalized( target );
		vec3_<T> c = cross_product( s, t );
		T d = dot_product( s, t );

		if ( equal( d, T( 1 ) ) ) // check if vectors are the same
			return quat_<T>::identity();

		auto clen = length( c );
		if ( equal( clen, T( 0 ) ) ) // check if vectors are 180 deg apart
			return quat_<T>( 0, 1, 0, 0 ); // this doesn't work if source is unit_x

		c /= clen;
		auto a = std::acos( d ) * T( 0.5 );
		auto sa = std::sin( a );

		return quat_<T>( std::cos( a ), c.x * sa, c.y * sa, c.z * sa );
	}

	XO_TEST_CASE( xo_quat_axes_test )
	{
		auto q = quat_from_euler( 20_degd, -30_degd, 40_degd );
		auto qx = local_x_axis( q );
		auto qy = local_y_axis( q );
		auto qz = local_z_axis( q );
		auto qx1 = q * vec3d::unit_x();
		auto qy1 = q * vec3d::unit_y();
		auto qz1 = q * vec3d::unit_z();
		auto qxyz = axes_from_quat( q );
		XO_CHECK( equal( qx, qx1 ) );
		XO_CHECK( equal( qy, qy1 ) );
		XO_CHECK( equal( qz, qz1 ) );
		XO_CHECK( qxyz.x == qx );
		XO_CHECK( qxyz.y == qy );
		XO_CHECK( qxyz.z == qz );
	}

	XO_TEST_CASE( xo_quat_from_directions )
	{
		auto eps = 1e-4f;
		random_number_generator_fast rng;
		for ( index_t i = 0; i < 10; ++i )
		{
			auto v1 = normalized( rng.uniform_vec3<float>( -1, 1 ) );
			auto v2 = normalized( rng.uniform_vec3<float>( -1, 1 ) );
			auto q1 = quat_from_directions( v1, v2 );
			auto q2 = quat_from_directions( v2, v1 );
			auto q3 = quat_from_directions( v2, -v2 );
			auto q4 = quat_from_directions( v1, v1 );
			auto q1o = quat_from_directions_old( v1, v2 );
			auto q2o = quat_from_directions_old( v2, v1 );
			auto q3o = quat_from_directions_old( v2, -v2 );
			auto q4o = quat_from_directions_old( v1, v1 );
			XO_CHECK( equal( q1, q1o, eps ) );
			XO_CHECK( equal( q2, q2o, eps ) );
			XO_CHECK( equal( q3, q3o, eps ) );
			XO_CHECK( equal( q4, q4o, eps ) );
		}
	}

	XO_TEST_CASE_SKIP( xo_rotation_around_axis_test )
	{
		for ( auto d = 0_degd; d < 180_degd; d += 10_degd ) {
			auto q = quat_from_euler( d, 10_degd, 0_degd, euler_order::yzx );
			auto a1 = degreed( rotation_around_axis_legacy( q, vec3d::unit_x() ) );
			auto a2 = degreed( rotation_around_axis_fast( q, vec3d::unit_x() ) );
			auto a4 = degreed( rotation_around_axis( q, vec3d::unit_x() ) );
			log::info( d, ": as=", a1, " af=", a2, " a4=", a4 );
		}

		random_number_generator_fast rng;
		for ( index_t i = 0; i < 10; ++i ) {
			auto q = quat_from_euler( 20_degd, -30_degd, 40_degd );
			auto v = normalized( rng.uniform_vec3<double>( -1, 1 ) );
			auto a1 = rotation_around_axis_legacy( q, v );
			auto a2 = rotation_around_axis_fast( q, v );
			auto a4 = rotation_around_axis( q, v );
		}
	}
}
