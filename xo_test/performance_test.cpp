#pragma once

#include <cmath>
#include "xo/time/stopwatch.h"
#include "xo/numerical/functions.h"
#include "xo/system/log.h"
#include "xo/container/prop_node.h"
#include "xo/system/test_framework.h"
#include "xo/numerical/random.h"
#include "xo/container/array.h"
#include "xo/numerical/bounds.h"
#include "xo/geometry/euler_angles.h"
#include "test_tools.h"

namespace xo
{
#ifdef XO_DEBUG
	constexpr static int num_evals = 1000000;
#else
	constexpr static int num_evals = 10000000;
#endif
	template< typename T > T test_func( T(*f)(T), T min = T(-1), T max = T(1) )
	{
		T res = 0, inc = ( max - min ) / T( num_evals ), x = min;
		for ( int i = num_evals; i > 0; --i, x += inc )
			res += f( x );
		return res;
	}

	template< typename T > T test_func( T( *f )( T, T ), T min = T( -1 ), T max = T( 1 ) )
	{
		T res = 0, inc = ( max - min ) / T( num_evals ), x = min;
		for ( int i = num_evals; i > 0; --i, x += inc )
			res += f( x, 0.1 );
		return res;
	}

	template< int N = 100 > array< quatf, N > make_random_quaternions() {
		random_number_generator rng;
		array< quatf, N > res;
		for ( int i = 0; i < N; ++i )
			res[ i ] = make_random_quat( euler_order::xyz, rng ).first;
		return res;
	}

	template< int N > vec3radf euler_test( stopwatch& sw )
	{
		vec3radf result;
		auto name = stringf( "euler_test%d", N );
		auto qa = make_random_quaternions<N>();
		sw.start();
		for ( int i = num_evals / N; i > 0; --i )
			for ( int j = 0; j < N; ++j )
				result += euler_yxz_from_quat( qa[ j ] );
		sw.add_measure( name );
		return result;
	}

	template< int N > radianf angle_test( stopwatch& sw )
	{
		radianf result;
		vec3f axis = normalized( vec3f( 1, 2, 3 ) );
		auto name = stringf( "angle_test%d", N );
		auto qa = make_random_quaternions<N>();
		sw.start();
		for ( int i = num_evals / N; i > 0; --i )
			for ( int j = 0; j < N; ++j )
				result += rotation_around_axis( qa[ j ], axis );
		sw.add_measure( name );
		return result;
	}

#define MEASURE_FUNC_1D( _cont_, _sw_, _func_, _min_, _max_ ) \
	{ _sw_.start(); \
	auto r = test_func<double>( &_func_, _min_, _max_ ); \
	_sw_.add_measure( #_func_ ); \
	_cont_.push_back( r ); }

	XO_TEST_CASE_SKIP( function_performance )
	{
		std::vector<double> res;
		res.reserve( 100 );
		stopwatch sw;

		MEASURE_FUNC_1D( res, sw, std::cos, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::acos, -1, 1 );
		MEASURE_FUNC_1D( res, sw, fast_acos<double>, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::sin, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::asin, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::tan, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::atan, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::atan2, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::sqrt, 0, 2 );
		euler_test<10>( sw );
		auto ve = euler_test<100>( sw );
		angle_test<10>( sw );
		angle_test<100>( sw );


		log::info( ve, "Function performance results:\n", sw.get_report() );
	}
}
