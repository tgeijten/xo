#pragma once

#include "xo/xo_types.h"
#include "xo/time/timer.h"
#include "xo/string/string_type.h"
#include "xo/container/vector_type.h"

namespace xo
{
	class XO_API stopwatch
	{
	public:
		stopwatch() : current_split_( 0 ) {}

		/// restarts the stopwatch. Existing splits are kept, subsequent splits are added to existing ones.
		void restart() { current_split_ = 0; timer_.restart(); }

		/// add a split with a specific tag. After a restart, splits must be performed in the same order.
		void split( const char* name ) {
			if ( current_split_ >= split_times_.size() ) {
				split_times_.emplace_back( timer_.restart() );
				split_names_.emplace_back( name );
				++current_split_;
			}
			else split_times_[ current_split_++ ] += timer_.restart();
		}

		/// get split information
		size_t size() const { return split_times_.size(); }
		const vector<time>& split_times() const { return split_times_; }
		const vector<string>& split_names() const { return split_names_; }

		void reserve( size_t n ) { split_times_.reserve( n ); split_names_.reserve( n ); }

		/// get report as a prop_node
		prop_node get_report( int decimals = 6 );
		
	private:
		timer timer_;
		index_t current_split_;
		vector<time> split_times_;
		vector<string> split_names_;
	};
}
