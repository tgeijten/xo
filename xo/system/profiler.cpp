﻿#include "profiler.h"
#include <algorithm>
#include <thread>
#include "xo/system/log_sink.h"
#include "profiler_config.h"
#include "xo/numerical/math.h"
#include "xo/container/prop_node.h"
#include "xo/string/string_tools.h"
#include "xo/container/prop_node_tools.h"

namespace xo
{
	profiler::profiler( bool auto_start ) :
		current_section_( nullptr ),
		enabled_( false ),
		overhead_estimate( time_from_seconds( 1000 ) ),
		instance_thread_()
	{
		if ( auto_start )
			start();
	}

	profiler::~profiler()
	{}

	profiler& profiler::instance()
	{
		static profiler inst( false );
		return inst;
	}

	void profiler::start( const char* label )
	{
		xo_error_if( enabled_ || current_section_ != nullptr, "profiler::start() called while profiler was already enabled" );

		instance_thread_ = std::this_thread::get_id();
		enabled_ = true;
		init_overhead_estimate();
		sections_.clear();

		current_section_ = add_section( label, no_index );
		current_section_->epoch = now();
	}

	void profiler::stop()
	{
		xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		xo_error_if( !enabled_ || current_section_ != root(), "profiler::stop() was called without call to profiler::start() in same scope" );

		// update root time
		root()->total_time = now() - root()->epoch;
		root()->count++;

#ifdef XO_PROFILER_MEASURE_OVERHEAD
		root()->overhead += overhead_estimate;
#endif
		enabled_ = false;
		current_section_ = nullptr;
	}

	profiler::section* profiler::start_section( const char* name )
	{
		auto t = now();
		//xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		current_section_ = acquire_section( name, current_section_->id );
		current_section_->epoch = t;
#ifdef XO_PROFILER_MEASURE_OVERHEAD
		current_section_->overhead += now() - t + overhead_estimate;
#endif
		return current_section_;
	}

	void profiler::end_section()
	{
#ifdef XO_PROFILER_MEASURE_OVERHEAD
		auto t1 = now();
#endif
		//xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		auto* prev_section = current_section_;
		current_section_ = &sections_[prev_section->parent_id];

#ifdef XO_PROFILER_MEASURE_OVERHEAD
		auto t2 = now();
		prev_section->total_time += t2 - prev_section->epoch;
		prev_section->overhead += ( t2 - t1 ) + overhead_estimate;
#else
		//prev_section->overhead += overhead_estimate;
		prev_section->total_time += now() - prev_section->epoch;
		prev_section->count++;
#endif
	}

	profiler::section* profiler::find_section( size_t id )
	{
		auto it = std::find_if( sections_.begin(), sections_.end(), [&]( section& s ) { return s.id == id; } );
		return it != sections_.end() ? &( *it ) : nullptr;
	}

	profiler::section* profiler::find_section( const char* name, size_t parent_id )
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

	prop_node profiler::report( double minimum_expand_percentage, bool add_log_level_tag )
	{
		prop_node pn;
		if ( enabled() )
			stop();
		if ( root() )
			report_section( root(), pn, minimum_expand_percentage, add_log_level_tag );
		return pn;
	}

	void profiler::report_section( section* s, prop_node& pn, double minimum_expand_percentage, bool add_log_level_tag )
	{
		double root_total = root()->total_time.milliseconds();
		double total_ms = s->total_time.milliseconds();
		double total_perc = 100.0 * total_ms / root_total;
		double excl_ms = exclusive_time( s ).milliseconds();
		double excl_perc = 100.0 * excl_ms / root_total;
		double excl_avg_ns = min( 999999.0, excl_ms / s->count * 1e6 ); // nanoseconds
		double over = total_overhead( s ).milliseconds();
		double over_perc = 100.0 * over / total_ms;

		string key;
		if ( add_log_level_tag )
		{
			if ( excl_perc < 1 ) key += "@2";
			else if ( excl_perc < 5 ) key += "@3";
			else if ( excl_perc < 15 ) key += "@4";
			else key += "@5";
		}

		auto value = stringf( "%6.0fms %6.2f%% (%5.2f%%) %6d %6.0fns ~%2.0f%% OH", total_ms, total_perc, excl_perc, s->count, excl_avg_ns, clamped( over_perc, 0.0, 99.0 ) );
		auto& child_pn = pn.add_key_value( key + s->name, value );
		if ( total_perc >= minimum_expand_percentage )
		{
			auto children = get_children( s->id );
			std::sort( children.begin(), children.end(), [&]( section* s1, section* s2 ) { return s1->total_time > s2->total_time; } );
			for ( auto& c : children )
				report_section( c, child_pn, minimum_expand_percentage, add_log_level_tag );
		}
	}

	void profiler::log_results( double minimum_expand_percentage )
	{
		log_prop_node( report( minimum_expand_percentage, true ) );
	}

	time profiler::exclusive_time( section* s )
	{
		auto t = s->total_time;
		for ( auto& cs : sections_ )
			if ( cs.parent_id == s->id )
				t -= cs.total_time;
		return t;
	}

	time profiler::total_overhead( section* s )
	{
#ifdef XO_PROFILER_MEASURE_OVERHEAD
		auto t = s->overhead;
#else
		auto t = overhead_estimate * s->count;
#endif
		for ( auto& cs : sections_ )
			if ( cs.parent_id == s->id )
				t += total_overhead( &cs );
		return t;
	}

	void profiler::init_overhead_estimate()
	{
#ifdef XO_PROFILER_MEASURE_OVERHEAD
		timer t;
		time t1, t2;
		t1 = now();
		for ( int i = 0; i < 9999; ++i )
			t2 = now();
		overhead_estimate = ( t2 - t1 ) / 10000;
#else
		sections_.clear();
		current_section_ = add_section( "init_overhead_estimate", no_index );
		start_section( "test_section1" );
		end_section();
		int samples = 10000;
		timer t;
		for ( int i = 0; i < samples; ++i )
		{
			start_section( "test_section1" );
			end_section();
		}
		auto est = t();
		overhead_estimate = std::min( overhead_estimate, est / samples );
#endif
	}
}
