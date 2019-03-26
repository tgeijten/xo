#include "timer_v2.h"

#include "xo/system/xo_config.h"

#if XO_USE_WINDOWS_PERFORMANCE_COUNTER
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <profileapi.h>
#else
#include <chrono>
#endif

namespace xo
{
#if XO_USE_WINDOWS_PERFORMANCE_COUNTER
	long long query_windows_performance_counter_frequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency( &frequency );
		return frequency.QuadPart;
	}

	const long long g_frequency = query_windows_performance_counter_frequency();

	time get_time()
	{
		LARGE_INTEGER count;
		QueryPerformanceCounter( &count );
		return time( count.QuadPart * 1'000'000'000 / g_frequency ); // #TODO check if this won't overflow
	}
#else
	time get_time()
	{
		auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
		return time( std::chrono::duration_cast<std::chrono::nanoseconds>( now ).count() );
	}
#endif

	timer_v2::timer_v2() :
	epoch_( get_time() )
	{}

	time timer_v2::operator()() const
	{
		return get_time() - epoch_;
	}

	void timer_v2::reset()
	{
		epoch_ = get_time();
	}
}
