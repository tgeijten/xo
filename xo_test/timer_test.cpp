#include "xo/system/system_tools.h"
#include "xo/system/log.h"
#include "xo/system/profiler_config.h"
#include <iomanip>
#include <vector>
#include <cmath>
#include "xo/time/timer.h"

namespace xo
{
	std::vector< double > work_result;

	void do_work( int amount )
	{
		XO_PROFILE_FUNCTION( xo::profiler::instance() );
		double sum = 0.0;
		for ( int x = 0; x < amount; ++x )
			sum += std::sqrt( double( x ) );
		work_result.push_back( sum );
	}

	void profile1()
	{
		XO_PROFILE_FUNCTION( xo::profiler::instance() );
		do_work( 10 );
	}

	void profile2( int i )
	{
		XO_PROFILE_FUNCTION( xo::profiler::instance() );
		do_work( 100 );
		if ( i % 5 < 3 )
			profile2( i + 1 );
		profile1();
	}

	void profile3( int i )
	{
		XO_PROFILE_FUNCTION( xo::profiler::instance() );
		do_work( 1000 );
		profile1();
		profile2( i );
		if ( i % 7 < 3 )
			profile3( i + 1 );
	}

	void profile_test()
	{
		xo_profiler_start();

		for ( int i = 0; i < 100000; ++i )
			profile3( i );

		log::info( "Profile report:\n", xo_profiler_report() );
	}
}
