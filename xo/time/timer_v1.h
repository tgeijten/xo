#pragma once

#include "xo/xo_types.h"
#include "xo/system/xo_api.h"
#include "xo/system/xo_config.h"

#include <chrono> // #TODO: try to move this to cpp

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
#if XO_USE_WINDOWS_PERFORMANCE_COUNTER
	struct windows_performance_counter_clock
	{
		using rep = long long;
		using period = std::nano;
		using duration = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< windows_performance_counter_clock >;
		static const bool is_steady = true;
		static time_point now();
	};
#endif

	class XO_API timer_v1
	{
	public:
#if XO_USE_WINDOWS_PERFORMANCE_COUNTER
		using timer_t = windows_performance_counter_clock;
		using tick_t = windows_performance_counter_clock::rep;
#else
		using timer_t = std::chrono::high_resolution_clock;
		using tick_t = long long;
#endif
		timer_v1() : epoch( timer_t::now() ) {}
		void reset() { epoch = timer_t::now(); }

		double seconds() const { return std::chrono::duration< double, std::ratio< 1 > >( elapsed() ).count(); }
		double minutes() const { return std::chrono::duration< double, std::ratio< 60 > >( elapsed() ).count(); }
		double hours() const { return std::chrono::duration< double, std::ratio< 3600 > >( elapsed() ).count(); }
		long long milliseconds() const { return std::chrono::duration_cast< std::chrono::milliseconds >( elapsed() ).count(); }
		long long microseconds() const { return std::chrono::duration_cast< std::chrono::microseconds >( elapsed() ).count(); }
		long long nanoseconds() const { return std::chrono::duration_cast< std::chrono::nanoseconds >( elapsed() ).count(); }
		tick_t ticks() const { return elapsed().count(); }

	private:
		timer_t::time_point epoch;
		timer_t::duration elapsed() const { return timer_t::now() - epoch; }
	};
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
