#pragma once

#include "xo/xo_types.h"
#include "xo/system/system_tools.h"
#include "xo/time/timer.h"
#include <thread>
#include <vector>

namespace xo
{
	class XO_API profiler
	{
	public:
		struct section
		{
			section( const char* n, size_t i, size_t pi ) : name( n ), id( i ), parent_id( pi ), total_time( 0 ), overhead( 0 ), count( 0 ) {}
			const char* name;
			size_t id;
			size_t parent_id;
			time total_time;
			time overhead;
			size_t count;
			time epoch;
		};

		void reset();
		section* start_section( const char* name );
		void end_section();
		time now() const { return timer_(); }
		prop_node report();
		static profiler& instance();

	private:
		profiler();
		void clear();

		void report_section( section* s, prop_node& pn );
		time exclusive_time( section* s );
		time total_overhead( section* s );
		section* root() { return &sections_.front(); }
		section* find_section( size_t id );
		section* find_section( const char* name, size_t parent_id );
		section* acquire_section( const char* name, size_t parent_id );
		section* add_section( const char* name, size_t parent_id );
		std::vector< section* > get_children( size_t parent_id );

		std::vector< section > sections_;
		timer timer_;
		section* current_section_;
		time overhead_estimate;
		std::thread::id instance_thread_;

		void init_overhead_estimate();
	};

	struct XO_API scoped_section_profiler
	{
		scoped_section_profiler( const char* name ) { profiler::instance().start_section( name ); }
		~scoped_section_profiler() { profiler::instance().end_section(); }
	};
}
