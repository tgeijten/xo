#include "stopwatch.h"

namespace xo
{
	void stopwatch::add_measure( const string& s )
	{
		auto now = timer_.seconds();
		auto iter = xo::find_if( measures_, [&]( measure_t& m ) { return m.first == s; } );
		if ( iter == measures_.end() )
			measures_.push_back( make_pair( s, now - epoch_ ) );
		else iter->second += now - epoch_;
		epoch_ = timer_.seconds();
		internal_measure_ += epoch_ - now;
	}

	xo::prop_node stopwatch::get_report( int decimals )
	{
		auto old_precision = set_to_str_precision( decimals );

		prop_node pn;
		for ( auto& m : measures_ )
			pn.push_back( m.first, m.second );
		pn.push_back( "internal", internal_measure_ );

		set_to_str_precision( old_precision );

		return pn;
	}
}
