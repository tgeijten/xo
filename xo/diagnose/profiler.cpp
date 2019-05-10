#include "profiler.h"
#include <algorithm>
#include <thread>
#include "xo/system/log_sink.h"
#include "profiler_config.h"
#include "xo/numerical/math.h"
#include "xo/container/prop_node.h"

namespace xo
{
	profiler::profiler( bool auto_start ) :
		current_section_( nullptr ),
		enabled_( false ),
		instance_thread_()
	{
		if ( auto_start )
			start();
	}

	profiler::~profiler()
	{}

	void profiler::clear_sections()
	{
		sections_.clear();
		current_section_ = nullptr;
		current_section_ = add_section( "TOTAL", no_index );
		current_section_->epoch = now();
	}

	void profiler::start()
	{
		instance_thread_ = std::this_thread::get_id();
		enabled_ = true;
		init_overhead_estimate();
		clear_sections();
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
		current_section_ = &sections_[ prev_section->parent_id ];

#ifdef XO_PROFILER_MEASURE_OVERHEAD
		auto t2 = now();
		prev_section->total_time += t2 - prev_section->epoch;
		prev_section->overhead += ( t2 - t1 ) + overhead_estimate;
#else
		prev_section->overhead += overhead_estimate;
		prev_section->total_time += now() - prev_section->epoch;
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

	prop_node profiler::report()
	{
		xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		root()->total_time = now() - root()->epoch;
		prop_node pn;
		report_section( root(), pn );
		return pn;
	}

	profiler& profiler::instance()
	{
		static profiler inst;
		return inst;
	}

	void profiler::report_section( section* s, prop_node& pn )
	{
		double root_total = root()->total_time.millisecondsd();
		double total = s->total_time.millisecondsd();
		double rel_total = 100.0 * total / root_total;
		double ex = exclusive_time( s ).millisecondsd();
		double rel_ex = 100.0 * ex / root_total;
		double over = total_overhead( s ).millisecondsd();
		double rel_over = 100.0 * over / total;

		pn[ s->name ] = stringf( "%6.0fms %6.2f%% (%5.2f%% exclusive ~%.0f%% overhead)", total, rel_total, rel_ex, clamped( rel_over, 0.0, 100.0 ) );

		auto children = get_children( s->id );
		std::sort( children.begin(), children.end(), [&]( section* s1, section* s2 ) { return s1->total_time > s2->total_time; } );
		for ( auto& c : children )
			report_section( c, pn[ s->name ] );
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
		auto t = s->overhead;
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
		clear_sections();
		int samples = 10000;
		timer t;
		auto t1 = t();
		for ( int i = 0; i < samples; ++i )
		{
			start_section( "test_section1" );
			end_section();
		}
		auto t2 = t();
		overhead_estimate = ( t2 - t1 ) / samples;
#endif
	}
}
