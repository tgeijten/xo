#pragma once
#include "xo/time/timer_v1.h"
#include "xo/system/system_tools.h"
#include "xo/system/log.h"
#include "xo/diagnose/profiler_config.h"
#include <iomanip>
#include "xo/time/timer.h"

namespace xo
{
	void timer_test()
	{
		long long delta = 0;
		const int samples = 10000000;
		timer_v1 t;
		for ( int i = 0; i < samples; ++i )
		{
			long long tick1 = t.ticks();
			long long tick2 = t.ticks();
			delta += tick2 - tick1;
		}
		log::info( "Time v1 measure takes ", (double)delta / samples, "ns" );

		timer t2;
		time delta2;
		for ( int i = 0; i < samples; ++i )
		{
			auto tick1 = t2();
			auto tick2 = t2();
			delta2 += tick2 - tick1;
		}
		log::info( "Time v2 measure takes ", (double)delta2.nanoseconds() / samples, "ns" );
	};

	std::vector< double > work_result;

	void do_work( int amount )
	{
		XO_PROFILE_FUNCTION;
		double sum = 0.0;
		for ( int x = 0; x < amount; ++x )
			sum += sqrt( double( x ) );
		work_result.push_back( sum );
	}

	void profile1()
	{
		XO_PROFILE_FUNCTION;
		do_work( 10 );
	}

	void profile2( int i )
	{
		XO_PROFILE_FUNCTION;
		do_work( 100 );
		if ( i % 5 < 3 )
			profile2( i + 1 );
		profile1();
	}

	void profile3( int i )
	{
		XO_PROFILE_FUNCTION;
		do_work( 1000 );
		profile1();
		profile2( i );
		if ( i % 7 < 3 )
			profile3( i + 1 );
	}

	void profile_test()
	{
		xo_profiler_reset();

		for ( int i = 0; i < 100000; ++i )
			profile3( i );

		log::info( "Profile report:\n", xo_profiler_report() );
	}
}
