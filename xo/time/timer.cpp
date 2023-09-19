#include "timer.h"

#include "xo/system/xo_config.h"
#include "xo/container/pair_type.h"
#include "xo/numerical/math.h"

#if XO_WINDOWS_PERFORMANCE_COUNTER_ENABLED
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <profileapi.h>
#else
#include <chrono>
#endif

#ifdef XO_ENABLE_CPU_CYCLE_COUNT
#ifdef XO_COMP_MSVC
#	include <intrin.h>
using cpu_cycle_count_t = unsigned long long;
inline cpu_cycle_count_t cpu_cycle_count() { return __rdtsc(); }
#else
#	include <x86intrin.h>
using cpu_cycle_count_t = unsigned long long;
#endif
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

	std::pair<long long, long long> get_time_from_ticks_factor() {
		long long num = 1'000'000'000;
		long long denom = query_windows_performance_counter_frequency();
		auto d = xo::greatest_common_divisor( num, denom );
		return { num / d, denom / d };
	}

	const auto g_time_from_ticks = get_time_from_ticks_factor();

	long long get_tick_count()
	{
		LARGE_INTEGER count;
		QueryPerformanceCounter( &count );
		return count.QuadPart;
	}

	time get_time_from_ticks( long long ticks )
	{
		return time( ticks * g_time_from_ticks.first / g_time_from_ticks.second ); // #todo check if this won't overflow
	}
	long long get_ticks_from_time( time t )
	{
		return t.nanoseconds() * g_time_from_ticks.second / g_time_from_ticks.first; // #todo check if this won't overflow
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
	long long get_ticks_from_time( time t )
	{
		auto nano_duration = std::chrono::nanoseconds( t.nanoseconds() );
		return std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>( nano_duration ).count();
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

	time timer::restart()
	{
		auto prev_epoch = epoch_;
		epoch_ = get_tick_count();
		return get_time_from_ticks( epoch_ - prev_epoch );
	}

	void timer::set( time t )
	{
		auto ticks = get_ticks_from_time( t );
		if ( is_running() )
			epoch_ = get_tick_count() - ticks;
		else epoch_ = -ticks;
	}

	long long timer::tick_count() const
	{
		return get_tick_count() - epoch_;
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
