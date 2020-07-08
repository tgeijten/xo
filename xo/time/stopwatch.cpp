#include "stopwatch.h"
#include "xo/container/container_tools.h"
#include "xo/string/string_tools.h"
#include "xo/container/prop_node.h"
#include "xo/utility/irange.h"

namespace xo
{
	void stopwatch::split( const char* s )
	{
		if ( current_split_ >= split_times_.size() )
		{
			split_times_.emplace_back( timer_.restart() );
			split_names_.emplace_back( s );
			current_split_++;
		}
		else split_times_[ current_split_++ ] += timer_.restart();
	}

	void stopwatch::restart()
	{
		current_split_ = 0;
		timer_.restart();
	}

	prop_node stopwatch::get_report( int decimals )
	{
		auto old_precision = set_to_str_precision( decimals );

		prop_node pn;
		for ( index_t idx : irange( split_count() ) )
			pn.add_key_value( split_names_[ idx ], split_times_[ idx ] );

		set_to_str_precision( old_precision );

		return pn;
	}
}
