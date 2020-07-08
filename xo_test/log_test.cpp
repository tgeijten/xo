#include "xo/system/test_case.h"
#include "xo/system/log.h"
#include "xo/string/string_tools.h"
#include "xo/geometry/vec3.h"
#include "xo/time/stopwatch.h"
#include "xo/container/prop_node.h"
#include <thread>

namespace xo
{
	void log_some_stuff( int n )
	{
		for ( int i = 0; i < n; ++i )
		{
			log::debug( "Starting log message ", i );
			log::info( "The square root of ", i, "", std::sqrt( double( i ) ) );
			log::warning( stringf( "Warning %d", i ) );
			log::error( "Error in vector: ", vec3f( 1.0f * i, 2.0f * i, 3.0f * i ) );
		}
	}

	XO_TEST_CASE_SKIP( xo_log_test )
	{
		// test single-threaded
		stopwatch sw;
		log_some_stuff( 2000 );
		sw.split( "single_threaded" );

		// test multi-thread
		std::vector<std::thread> threads;
		for ( int i = 0; i < 10; ++i )
			threads.emplace_back( &log_some_stuff, 200 );
		for ( auto& t : threads )
			t.join();
		sw.split( "multi_threaded" );

		// report results
		log::info( "RESULTS\n", sw.get_report() );
	}
}
