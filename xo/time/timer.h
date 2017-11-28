#pragma once

#include "xo/system/platform.h"
#include <chrono>

#if defined (_MSC_VER ) && ( _MSC_VER <= 1800 ) // MSVC 2013 and lower do not have proper chrono support
#	define XO_USE_WINDOWS_PERFORMANCE_COUNTER
#endif

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
#if defined XO_USE_WINDOWS_PERFORMANCE_COUNTER
	struct windows_performance_counter_clock
	{
		typedef long long rep;
		typedef std::nano period;
		typedef std::chrono::duration< rep, period > duration;
		typedef std::chrono::time_point< windows_performance_counter_clock > time_point;
		static const bool is_steady = true;
		static time_point now();
	};
#endif

	class XO_API timer
	{
	public:
#if defined XO_USE_WINDOWS_PERFORMANCE_COUNTER
		typedef windows_performance_counter_clock timer_t;
#else
		typedef std::chrono::high_resolution_clock timer_t;
#endif
		timer() : epoch( timer_t::now() ) {}
		void reset() { epoch = timer_t::now(); }

		double seconds() const { return std::chrono::duration< double, std::ratio< 1 > >( elapsed() ).count(); }
		double minutes() const { return std::chrono::duration< double, std::ratio< 60 > >( elapsed() ).count(); }
		double hours() const { return std::chrono::duration< double, std::ratio< 3600 > >( elapsed() ).count(); }
		long long milliseconds() const { return std::chrono::duration_cast< std::chrono::milliseconds >( elapsed() ).count(); }
		long long microseconds() const { return std::chrono::duration_cast< std::chrono::microseconds >( elapsed() ).count(); }
		long long nanoseconds() const { return std::chrono::duration_cast< std::chrono::nanoseconds >( elapsed() ).count(); }
		long long ticks() { return elapsed().count(); }

	private:
		timer_t::time_point epoch;
		timer_t::duration elapsed() const { return timer_t::now() - epoch; }
	};
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
