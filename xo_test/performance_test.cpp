#pragma once

#include <math.h>
#include "xo/time/stopwatch.h"
#include "xo/numerical/functions.h"
#include "xo/system/log.h"
#include "xo/container/prop_node.h"

namespace xo
{
	template< typename T > T test_func( T(*f)(T), T min = T(-1), T max = T(1), T inc = T(1e-7) )
	{
		T result = 0;
		for ( T x = min; x <= max; x += inc )
			result += f( x );
		return result;
	}

#define TEST_FUNCTION_PERFORMANCE( _cont_, _sw_, _func_ ) \
	_cont_.push_back( test_func< double >( &_func_ ) ); _sw_.add_measure( #_func_ );

	void performance_test()
	{
		std::vector< double > res;
		res.reserve( 100 );
		stopwatch sw;

		TEST_FUNCTION_PERFORMANCE( res, sw, ::acos );
		TEST_FUNCTION_PERFORMANCE( res, sw, fast_acos< double > );
		TEST_FUNCTION_PERFORMANCE( res, sw, ::cos );
		TEST_FUNCTION_PERFORMANCE( res, sw, ::sin );
		TEST_FUNCTION_PERFORMANCE( res, sw, ::asin );

		log::info( sw.get_report() );
	}
}
