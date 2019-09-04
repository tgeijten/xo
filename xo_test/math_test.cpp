#pragma once

#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "xo/container/flag_set.h"
#include "xo/container/storage.h"
#include "xo/filesystem/filesystem.h"
#include "xo/geometry/angle.h"
#include "xo/geometry/euler_angles.h"
#include "xo/geometry/quat.h"
#include "xo/geometry/vec3.h"
#include "xo/numerical/math.h"
#include "xo/numerical/random.h"
#include "xo/numerical/regression.h"
#include "xo/numerical/regular_piecewise_linear_function.h"
#include "xo/shape/aabb.h"
#include "xo/string/string_tools.h"
#include "xo/system/log.h"
#include "xo/system/system_tools.h"
#include "xo/system/test_case.h"
#include "xo/time/timer.h"
#include "xo/utility/irange.h"
#include "xo/utility/optional.h"

using std::cout;
using std::endl;

namespace xo
{
	double sin_func( double x ) { return std::sin( x ); }

	XO_TEST_CASE( xo_math_test_test )
	{
		aabbf bb;
		bb += vec3f( -1, 2, -3 );
		bb += vec3f( 3, -2, 1 );
		XO_CHECK( bb.lower_bounds == vec3f( -1, -2, -3 ) );
		XO_CHECK( bb.upper_bounds == vec3f( 3, 2, 1 ) );
	}

	XO_TEST_CASE( xo_optional_test )
	{
		optional< char > a;
		XO_CHECK( sizeof( a ) == sizeof( char ) );
		XO_CHECK( !a );
		a = 123;
		XO_CHECK( !!a );
		XO_CHECK( *a == 123 );
		a = -128;
		XO_CHECK( !a );

		optional< double > b;
		XO_CHECK( sizeof( b ) == sizeof( double ) );
		XO_CHECK( !b );
		b = 1.234;
		XO_CHECK( !!b );
		XO_CHECK( *b == 1.234 );
		b = std::numeric_limits< double >::quiet_NaN();
		XO_CHECK( !b );

		optional< vec3f > c;
		XO_CHECK( sizeof( c ) != sizeof( vec3f ) );
		XO_CHECK( !c );
		c = vec3f( 1, 2, 3 );
		XO_CHECK( !!c );
		XO_CHECK( *c == vec3f( 1, 2, 3 ) );
		optional< vec3d > d( vec3f( 3, 4, 5 ) );
		XO_CHECK( !!d );
	}

	XO_TEST_CASE( xo_function_test )
	{
		auto func = regular_piecewise_linear_function< double, 16 >( -constantsd::pi(), constantsd::pi(), sin_func );
		double diff = 0.0;
		int n = 0;
		for ( double x = -constants<double>::pi(); x <= constants<double>::pi(); x += 0.001 )
		{
			++n;
			auto f1 = func( x );
			auto f2 = sin_func( x );
			diff += fabs( f1 - f2 );
		}
		diff /= n;
		XO_CHECK_MESSAGE( diff < 0.01, stringf( "diff=%f", diff ) );
	}

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

		vec3_< degreef > deg_vec( degreef( 10 ), degreef( 20 ), degreef( 30 ) );
		vec3_< radianf > rad_vec;
		rad_vec = deg_vec;
		XO_CHECK( equal( rad_vec[ 0 ].value, radianf( degreef( 10 ) ).value ) );


		auto qtest = quat_from_euler( 180.0_deg, 180_deg, 180_deg, euler_order::xyz );
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

	void xo_clamp_test()
	{
		storage< double > sto;
		for ( double x = -100; x < 100; x += 1 )
		{
			auto f = sto.add_frame();
			double a = soft_clamped( x, 10.0, 20.0, 0.1 );
			double b = soft_clamped( x, -50.0, 50.0, 0.1 );
			double c = soft_clamped( x, -200.0, 0.0, 0.1 );
			f[ "x" ] = x;
			f[ "10..20" ] = a;
			f[ "-50..50" ] = b;
			f[ "-200..0" ] = c;
		}
		//std::ofstream( "X:/clamp_test.txt" ) << sto;
	}

	void xo_wrap_test()
	{
		for ( double x = -10; x < 10; x += 0.1 )
			log::info( x, ":\t", wrapped( x, 0.0, 1.0 ), "\t", wrapped( x, -1.5, 2.5 ), "\t", wrapped( x, -3.5, -1.5 ) );
	}

	XO_TEST_CASE( xo_linear_regression_test )
	{
#if 0
		double step = 1;
		for ( double i = 1; i <= 75.0; i += step ) {
			double tot1 = 0.0;
			double avg = ( i - 1 ) / 2;
			for ( double v = 0; v < i; v += step )
				tot1 += squared( v - avg );
			auto hi = 0.5 * ( i - 1 );
			auto tot1_alg = hi * ( hi + step ) * ( 2 * hi + step ) / ( 3 * step );
			printf( "%.2f: %8.5f %8.5f %8.5f %8.5f\n", i, tot1, tot1_alg, tots, tots_alg );
		}
#endif

		std::vector< double > x;
		std::vector< double > y;
		for ( int i = -100; i < 50; ++i )
		{
			x.push_back( i );
			y.push_back( i * 2.5 + 4.0 );
		}

		//xo::linear_regression lg( x.begin(), x.end(), y.begin(), y.end() );

		auto lg1 = linear_regression( x.begin(), x.end(), y.begin(), y.end() );
		XO_CHECK( equal( lg1[ 1 ], 2.5 ) );
		XO_CHECK( equal( lg1[ 0 ], 4.0 ) );

		x.clear();
		y.clear();
		auto xr = make_irange( -50, 100, 2 );
		for ( auto xre : xr )
			y.push_back( xre * -1.5 - 100 );

		auto lg2 = linear_regression( xr, y );
		XO_CHECK( equal( lg2[ 1 ], -1.5 ) );
		XO_CHECK( equal( lg2[ 0 ], -100.0 ) );

		auto lg3 = linear_regression( y, -50.0, 2.0 );
		XO_CHECK( equal( lg3[ 1 ], -1.5 ) );
		XO_CHECK( equal( lg3[ 0 ], -100.0 ) );

		auto x0 = intersect_y( lg3, 0.0 );
		auto x1 = intersect_y( lg3, 10.0 );
		auto x2 = intersect_y( lg3, -10.0 );
		XO_CHECK( equal( lg3( x0 ), 0.0 ) );
		XO_CHECK( equal( lg3( x1 ), 10.0 ) );
		XO_CHECK( equal( lg3( x2 ), -10.0 ) );
	}

#if 0
	void vec_quat_test()
	{
		//for ( size_t x = 0; x < 2000; ++x )
		//	if ( is_power_of_two( x ) )
		//		log::debug( x, " is a power of 2" );

		enum TestEnum { Apple, Pear, Banana, Orange };
		flag_set< TestEnum > flg;
		flg.set( Apple, true );
		flg.set( Banana, true );
		xo_logvar4( flg.get( Apple ), flg.get( Pear ), flg.get( Banana ), flg.get( Orange ) );
		flg.set( Banana, false );
		flg.set( Orange, true );
		xo_logvar4( flg.get<Apple>(), flg.get( Pear ), flg.get( Banana ), flg.get( Orange ) );

		std::default_random_engine re( 123 );
		std::uniform_real_distribution<> rd( -10, 10 );

		for ( size_t experiment = 0; experiment < 2; ++experiment )
		{
			std::vector< double > rv( 16 );
			std::generate( rv.begin(), rv.end(), [&]() { return rd( re ); } );

			auto xo_v1 = vec3( rv[ 0 ], rv[ 1 ], rv[ 2 ] );
			auto xo_v2 = vec3( rv[ 3 ], rv[ 4 ], rv[ 5 ] );
			auto xo_x = normalized( xo_v2 - xo_v1 );
			auto xo_y = normalized( cross_product( xo_x, normalized( xo_v2 ) ) );
			auto xo_z = cross_product( xo_x, xo_y );
			xo_logvar3( length( xo_x ), length( xo_y ), length( xo_z ) );
			auto xo_q1 = quat_from_axes( xo_x, xo_y, xo_z );
			auto xo_q2 = quat_from_axis_angle( normalized( vec3( rv[ 6 ], rv[ 7 ], rv[ 8 ] ) ), rad( rv[ 9 ] ) );
			auto xo_q3 = xo_q1 * xo_q2;
			auto xo_q4 = normalized( xo_q3 );
			xo_logvar3( length( xo_q1 ), length( xo_q2 ), length( xo_q3 ) );
			auto xo_v4 = xo_q4 * xo_v1;
			auto xo_v5 = rotation_vector_from_quat( xo_q4 );

			auto scone_v1 = scone::Vec3( rv[ 0 ], rv[ 1 ], rv[ 2 ] );
			auto scone_v2 = scone::Vec3( rv[ 3 ], rv[ 4 ], rv[ 5 ] );
			auto scone_x = ( scone_v2 - scone_v1 ).GetNormalized();
			auto scone_y = ( scone_x.GetCrossProduct( scone_v2.GetNormalized() ) ).GetNormalized();
			auto scone_z = scone_x.GetCrossProduct( scone_y );
			auto scone_q1 = QuatFromAxes( scone_x, scone_y, scone_z );
			auto scone_q2 = QuatFromAxisAngle( scone::Vec3( rv[ 6 ], rv[ 7 ], rv[ 8 ] ).GetNormalized(), scone::Radian( rv[ 9 ] ) );
			auto scone_q3 = scone_q1 * scone_q2;
			auto scone_q4 = scone_q3.GetNormalized();
			auto scone_v4 = scone_q4 * scone_v1;
			auto scone_v5 = scone_q4.ToExponentialMap2();

			compare( xo_v1, scone_v1 );
			compare( xo_v2, scone_v2 );
			compare( xo_x, scone_x );
			compare( xo_y, scone_y );
			compare( xo_z, scone_z );
			compare( xo_q1, scone_q1 );
			compare( xo_q2, scone_q2 );
			compare( xo_q3, scone_q3 );
			compare( xo_v4, scone_v4 );
			compare( xo_v5, scone_v5 );

			// euler angle test
			euler_order xo_eo[] = { euler_order::xyz, euler_order::xzy, euler_order::yxz, euler_order::yzx, euler_order::zxy, euler_order::zyx };
			scone::EulerOrder scone_eo[] = { scone::EULER_ORDER_XYZ, scone::EULER_ORDER_XZY, scone::EULER_ORDER_YXZ, scone::EULER_ORDER_YZX, scone::EULER_ORDER_ZXY, scone::EULER_ORDER_ZYX };

			for ( int j = 0; j < 6; ++j )
			{
				auto xo_qeo = quat_from_euler( rad( rv[ 0 ] ), rad( rv[ 1 ] ), rad( rv[ 2 ] ), xo_eo[ j ] );
				auto scone_qeo = QuatFromEuler( Radian( rv[ 0 ] ), Radian( rv[ 1 ] ), Radian( rv[ 2 ] ), scone_eo[ j ] );
				compare( xo_qeo, scone_qeo );
			}
		}
#endif
}
