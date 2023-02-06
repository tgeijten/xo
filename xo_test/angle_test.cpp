#include "xo/system/test_case.h"
#include "xo/geometry/angle.h"
#include "xo/geometry/quat.h"

namespace xo
{
	XO_TEST_CASE( xo_angle_test )
	{
		//auto ang1 = degree( 45 );
		//auto ang2 = radian( ang1 );
		//auto ang3 = ang1 + degree( 180 );

		auto a1_rd = radiand( constants<double>::half_pi() );
		auto a2_dd = degreed( 180.0 );
		auto a3 = a1_rd + radiand( a2_dd );
		auto a4 = a2_dd + degreed( a1_rd );

		auto a5_rf = radianf( ( 90_degf ) );
		auto a6_df = degreef( radiand( constants<float>::half_pi() ) );
		XO_CHECK( equal( a5_rf, radianf( constants<float>::half_pi() ), radianf( constantsf::ample_epsilon() ) ) );
		XO_CHECK( equal( a6_df, 90_degf, degreef( constantsf::ample_epsilon() ) ) );
		XO_CHECK( equal( degreef( a5_rf ), 90_degf, degreef( constantsf::ample_epsilon() ) ) );

		auto a1s = 0.5 * a1_rd;
		auto a2s = 0.5 * a2_dd;
		auto a3s = 2.0 * a3;

		auto sin1 = sin( a1_rd );
		auto sin4 = sin( a4 );

		vec3_< degreef > deg_vec( 10_degf, 20_degf, 30_degf );
		vec3_< radianf > rad_vec( vec3degf( 10_degf, 20_degf, 30_degf ) );
		rad_vec = vec3radf( deg_vec );
		XO_CHECK( equal( rad_vec[0].value, radianf( degreef( 10 ) ).value ) );

		auto qtest = quat_from_euler( 180.0_degd, 180_degd, 180_degd, euler_order::xyz );
		auto qtest2 = quat_from_axis_angle( vec3_<float>::unit_x(), a6_df );
		auto qtest3 = quat_from_axis_angle( vec3_<double>::unit_y(), a1_rd );
		auto p = axis_angle_from_quat( qtest2 );
		XO_CHECK( equal( p.first, vec3f::unit_x() ) );
		XO_CHECK( equal( p.second, radianf( a6_df ), radianf( constants<float>::ample_epsilon() ) ) );

		radian_<float> r1( 1 );
		degreed dd( 90 );
		radianf r2;
		r2 = radianf( dd );

		auto r3 = r1 + r2;
		XO_CHECK( r1 < r2 );
	}
}
