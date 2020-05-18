#include "timer.h"

#include "xo/system/xo_config.h"

#if XO_WINDOWS_PERFORMANCE_COUNTER_ENABLED
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <profileapi.h>
#else
#include <chrono>
#endif

namespace xo
{
#if XO_WINDOWS_PERFORMANCE_COUNTER_ENABLED
	long long query_windows_performance_counter_frequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency( &frequency );
		return frequency.QuadPart;
	}

	const long long g_frequency = query_windows_performance_counter_frequency();

	long long get_tick_count()
	{
		LARGE_INTEGER count;
		QueryPerformanceCounter( &count );
		return count.QuadPart;
	}

	time get_time_from_ticks( long long ticks )
	{
		return time( ticks * 1'000'000'000 / g_frequency ); // #todo check if this won't overflow
	}
#else
	long long get_tick_count()
	{
		return std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}

	time get_time_from_ticks( long long ticks )
	{
		auto ticks_duration = std::chrono::high_resolution_clock::duration( ticks );
		return time( std::chrono::duration_cast<std::chrono::nanoseconds>( ticks_duration ).count() );
	}
#endif

	timer::timer( bool start ) :
		epoch_( start ? get_tick_count() : 0 )
	{}

	time timer::operator()() const
	{
		auto current_time = epoch_ > 0 ? get_tick_count() - epoch_ : -epoch_;
		return get_time_from_ticks( current_time );
	}

	void timer::restart()
	{
		epoch_ = get_tick_count();
	}

	void timer::restart_and_pause()
	{
		epoch_ = 0;
	}

	time timer::pause()
	{
		// store current time in epoch
		epoch_ = -( get_tick_count() - epoch_ );
		return get_time_from_ticks( -epoch_ );
	}

	void timer::resume()
	{
		epoch_ = get_tick_count() + epoch_;
	}
}
