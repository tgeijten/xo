#pragma once

#include "timer.h"
#include "xo/xo_types.h"
#include "xo/string/string_type.h"

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	class XO_API stopwatch
	{
	public:
		using measure_t = std::pair< std::string, seconds_t >;

		stopwatch() : internal_measure_( 0 ), timer_() { epoch_ = timer_.seconds(); }
		~stopwatch() {}

		/// add a measure with a specific tag. identically names measures are summed
		void add_measure( const string& s );

		/// get report as a prop_node
		prop_node get_report( int decimals = 6 );
		
	private:
		timer timer_;
		std::vector< measure_t > measures_;
		seconds_t internal_measure_;
		seconds_t epoch_;
	};
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
