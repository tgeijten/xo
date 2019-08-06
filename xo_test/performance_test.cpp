#pragma once

#include <cmath>
#include "xo/time/stopwatch.h"
#include "xo/numerical/functions.h"
#include "xo/system/log.h"
#include "xo/container/prop_node.h"
#include "xo/system/test_framework.h"

namespace xo
{
#ifdef XO_DEBUG
	constexpr static double num_evals = 1e6;
#else
	constexpr static double num_evals = 1e7;
#endif
	template< typename T > T test_func( T(*f)(T), T min = T(-1), T max = T(1) )
	{
		T result = 0;
		T inc = ( max - min ) / T( num_evals );
		for ( T x = min; x <= max; x += inc )
			result += f( x );
		return result;
	}

	template< typename T > T test_func( T( *f )( T, T ), T min = T( -1 ), T max = T( 1 ) )
	{
		T result = 0;
		T inc = ( max - min ) / T( num_evals );
		for ( T x = min; x <= max; x += inc )
			result += f( x, 0.1 );
		return result;
	}

	template< int N = 100 > std::array< quatf, N > make_random_quaternions() {
		xo::rng
	}

#define MEASURE_FUNC_1D( _cont_, _sw_, _func_, _min_, _max_ ) \
	{ _sw_.start(); \
	auto r = test_func<double>( &_func_, _min_, _max_ ); \
	_sw_.add_measure( #_func_ ); \
	_cont_.push_back( r ); }

	XO_TEST_CASE( function_performance )
	{
		std::vector<double> res;
		res.reserve( 100 );
		stopwatch sw;

		MEASURE_FUNC_1D( res, sw, std::sqrt, 0, 2 );
		MEASURE_FUNC_1D( res, sw, std::cos, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::acos, -1, 1 );
		MEASURE_FUNC_1D( res, sw, fast_acos<double>, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::sin, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::asin, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::tan, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::atan, -1, 1 );
		MEASURE_FUNC_1D( res, sw, std::atan2, -1, 1 );

		log::info( "Function performance results:\n", sw.get_report() );
	}
}
