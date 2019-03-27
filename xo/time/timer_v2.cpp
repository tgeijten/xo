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
	namespace v2
	{
#if XO_USE_WINDOWS_PERFORMANCE_COUNTER
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
			return time( ticks * 1'000'000'000 / g_frequency ); // #TODO check if this won't overflow
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

		timer::timer() :
			epoch_( get_tick_count() )
		{}

		time timer::operator()() const
		{
			return get_time_from_ticks( get_tick_count() - epoch_ );
		}

		void timer::reset()
		{
			epoch_ = get_tick_count();
		}
	}
}
