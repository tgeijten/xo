#include "profiler.h"
#include <algorithm>
#include <thread>

namespace xo
{
	profiler profiler::instance_;
	std::thread::id instance_thread_;

	profiler::profiler()
	{
		instance_thread_ = std::this_thread::get_id();
		reset();
	}

	void profiler::reset()
	{
		duration_of_now = 0;
		sections_.clear();
		current_section_ = add_section( "TOTAL", no_index );
		current_section_->epoch = now();
	}

	profiler::section* profiler::start_section( const char* name )
	{
		auto t = now();
		xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		current_section_ = acquire_section( name, current_section_->id );
		current_section_->epoch = t;
		current_section_->overhead += now() - t + 2 * duration_of_now;

		return current_section_;
	}

	void profiler::end_section()
	{
		auto t = now();
		xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		auto* s = &sections_[ current_section_->parent_id ];
		current_section_->total_time += t - current_section_->epoch;
		current_section_->overhead += duration_of_now;
		current_section_ = s;
	}

	xo::profiler::section* profiler::find_section( size_t id )
	{
		auto it = std::find_if( sections_.begin(), sections_.end(), [&]( section& s ) { return s.id == id; } );
		return it != sections_.end() ? &( *it ) : nullptr;
	}

	xo::profiler::section* profiler::find_section( const char* name, size_t parent_id )
	{
		auto it = std::find_if( sections_.begin() + parent_id + 1, sections_.end(), [&]( section& s ) { return s.name == name && s.parent_id == parent_id; } );
		return it != sections_.end() ? &( *it ) : nullptr;
	}

	profiler::section* profiler::add_section( const char* name, size_t parent_id )
	{
		sections_.emplace_back( name, sections_.size(), parent_id );
		return &sections_.back();
	}

	profiler::section* profiler::acquire_section( const char* name, size_t parent_id )
	{
		if ( section* s = find_section( name, parent_id ) )
			return s;
		else return add_section( name, parent_id );
	}

	std::vector< profiler::section* > profiler::get_children( size_t parent_id )
	{
		std::vector< profiler::section* > children;
		for ( auto& s : sections_ )
			if ( s.parent_id == parent_id ) children.push_back( &s );
		return children;
	}

	prop_node profiler::report()
	{
		xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		root()->total_time = now() - root()->epoch;
		prop_node pn;
		report_section( root(), pn );
		return pn;
	}

	void profiler::report_section( section* s, prop_node& pn )
	{
		double root_total = root()->total_time / 1e6;
		double total = s->total_time / 1e6;
		double rel_total = 100.0 * total / root_total;
		double ex = exclusive_time( s ) / 1e6;
		double rel_ex = 100.0 * ex / root_total;
		double over = total_overhead( s ) / 1e6;
		double rel_over = 100.0 * over / total;

		pn[ s->name ] = stringf( "%6.0fms %6.2f%% (%5.2f%% exclusive %5.2f%% overhead)", total, rel_total, rel_ex, rel_over );

		auto children = get_children( s->id );
		std::sort( children.begin(), children.end(), [&]( section* s1, section* s2 ) { return s1->total_time > s2->total_time; } );
		for ( auto& c : children )
			report_section( c, pn[ s->name ] );
	}

	xo::nanoseconds_t profiler::exclusive_time( section* s )
	{
		nanoseconds_t t = s->total_time;
		for ( auto& cs : sections_ )
			if ( cs.parent_id == s->id )
				t -= cs.total_time;
		return t;
	}

	xo::nanoseconds_t profiler::total_overhead( section* s )
	{
		nanoseconds_t t = s->overhead;
		for ( auto& cs : sections_ )
			if ( cs.parent_id == s->id )
				t += total_overhead( &cs );
		return t;
	}
}
