#pragma once

#include "timer.h"
#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include <vector>

namespace xo
{
	class XO_API stopwatch
	{
	public:
		stopwatch() : current_split_( 0 ) {}

		/// restarts the stopwatch. Existing splits are kept, subsequent splits are added to existing ones.
		void restart();

		/// add a split with a specific tag. After a restart, splits must be performed in the same order.
		void split( const char* name );
		void split( const std::string& name ) { split( name.c_str() ); }

		/// get split information
		size_t split_count() const { return split_times_.size(); }
		const std::vector<time>& split_times() const { return split_times_; }
		const std::vector<string>& split_names() const { return split_names_; }

		void reserve( size_t n ) { split_times_.reserve( n ); split_names_.reserve( n ); }

		/// get report as a prop_node
		prop_node get_report( int decimals = 6 );
		
	private:
		timer timer_;
		index_t current_split_;
		std::vector<time> split_times_;
		std::vector<string> split_names_;
	};
}
