#include "profiler.h"
#include <algorithm>
#include <thread>
#include "xo/system/log_sink.h"
#include "profiler_config.h"

namespace xo
{
	profiler profiler::instance_;
	std::thread::id instance_thread_;

	void assign_test()
	{
		int apples[ 4 ];
		apples[ 0 ] = rand() % 100;
		apples[ 1 ] = rand() % 100;
		apples[ 2 ] = rand() % 100;
		apples[ 3 ] = rand() % 100;

		long long value = rand();
		long long bladie = 10;
		for ( int i = 0; i < 10000; ++i )
			bladie += value;
	}

	profiler::profiler()
	{
		instance_thread_ = std::this_thread::get_id();
		reset();
	}

	void profiler::clear()
	{
		sections_.clear();
		current_section_ = nullptr;
		current_section_ = add_section( "TOTAL", no_index );
		current_section_->epoch = now();
	}

	void profiler::reset()
	{
		timer t;
		tick_t t1, t2, t3;
		t1 = now();
		assign_test();
		t2 = now();
		for ( int i = 0; i < 999; ++i )
			t3 = now();
		auto ns = t.nanoseconds();

		duration_of_now = ( t3 - t2 - ( t2 - t1 ) ) / 1000;
		log::info( "duration of now=", duration_of_now, " total=", ( t3 - t2 ), " assign=", ( t2 - t1 ), " ns=", ns / 1000.0 );

		clear();

		section_duration = estimate_section_overhead();
		log::info( "section duration=", section_duration );

		clear();
	}

	profiler::section* profiler::start_section( const char* name )
	{
		auto t = now();
		xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		current_section_ = acquire_section( name, current_section_->id );
		current_section_->epoch = t;
		current_section_->overhead += now() - t + 3 * duration_of_now / 2;

		return current_section_;
	}

	void profiler::end_section()
	{
		xo_assert_msg( instance_thread_ == std::this_thread::get_id(), "Invalid thread ID" );
		auto* prev_section = current_section_;
		current_section_ = &sections_[ prev_section->parent_id ];
		//prev_section->overhead += section_duration;
		prev_section->total_time += now() - prev_section->epoch;
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

	profiler::tick_t profiler::exclusive_time( section* s )
	{
		nanoseconds_t t = s->total_time;
		for ( auto& cs : sections_ )
			if ( cs.parent_id == s->id )
				t -= cs.total_time;
		return t;
	}

	profiler::tick_t profiler::total_overhead( section* s )
	{
		nanoseconds_t t = s->overhead;
		for ( auto& cs : sections_ )
			if ( cs.parent_id == s->id )
				t += total_overhead( &cs );
		return t;
	}

	void section_test_function1( profiler& p, int c, int& samples );
	void section_test_function2( profiler& p, int c, int& samples )
	{
		p.start_section( "test2" );
		for ( int i = 0; i < c; ++i )
			section_test_function1( p, c / 4, samples );
		++samples;
		p.end_section();
	}

	void section_test_function1( profiler& p, int c, int& samples )
	{
		p.start_section( "test1" );
		for ( int i = 0; i < c; ++i )
			section_test_function2( p, c / 2, samples );
		++samples;
		p.end_section();
	}

	profiler::tick_t profiler::estimate_section_overhead()
	{
		int samples = 0;
		auto t1 = instance().now();
		for ( int i = 0; i < 10000; ++i )
			section_test_function1( instance(), 10, samples );
		auto t2 = instance().now();
		auto n = ( t2 - t1 ) / samples;
		log::info( instance().report() );
		log::info( "section duration: ", t2 - t1, " / ", samples, " = ", n );
		return n;
	}
}
