#pragma once

#include "xo/system/platform.h"
#include "xo/system/error_code.h"
#include "xo/system/version.h"
#include "xo/filesystem/path.h"

namespace xo
{
	XO_API version get_xo_version();
	XO_API char wait_for_key();
	XO_API string get_date_time_str( const char* format = "%Y-%m-%d_%H.%M.%S" );
	XO_API void crash( const string& message = "" );
	XO_API void sleep( int milliseconds );
	XO_API string tidy_identifier( const string& id );
	XO_API string clean_type_name( const char* name );
	template< typename T > string clean_type_name() { return clean_type_name( typeid( T ).name() ); }
	enum class thread_priority { lowest = -2, low = -1, normal = 0, high = 1, highest = 2 };
	XO_API void set_thread_priority( thread_priority p );

#ifdef XO_ENABLE_CPU_CYCLE_COUNT
#ifdef XO_COMP_MSVC
#	include <intrin.h>
	using cpu_cycle_count_t = unsigned long long;
	inline cpu_cycle_count_t cpu_cycle_count() { return __rdtsc(); }
	inline cpu_cycle_count_t cpu_cycle_frequency() { return __rdtsc(); }
#else
#	include <x86intrin.h>
	using cpu_cycle_count_t = unsigned long long;
	inline cpu_cycle_count_t cpu_cycle_count() { return __rdtsc(); }
#endif
#endif
}
