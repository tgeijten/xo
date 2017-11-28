#pragma once

#include "xo/utility/types.h"
#include "timer.h"
#include "xo/system/log.h"
#include "xo/container/prop_node.h"

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	class XO_API stopwatch
	{
	public:
		typedef std::pair< std::string, seconds_t > measure_t;

		stopwatch() : internal_measure_( 0 ), timer_() { start_first_measure(); }
		~stopwatch() {}

		/// reset timer for first measurement
		void start_first_measure() { epoch_ = timer_.seconds(); }

		/// add a measure with a specific tag. identically names measures are summed
		void add_measure( const string& s );

		/// get report as a prop_node
		prop_node get_report( int decimals = 6 );
		
	private:
		seconds_t epoch_;
		std::vector< measure_t > measures_;
		seconds_t internal_measure_;
		timer timer_;
	};
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
