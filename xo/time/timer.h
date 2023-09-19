#pragma once

#include "xo/system/xo_config.h"
#include "xo/time/time.h"

namespace xo
{
	struct XO_API timer
	{
		timer( bool start = true );
		time operator()() const;

		time pause();
		void resume();
		time restart();
		void set( time t );

		bool is_running() const { return epoch_ > 0; }
		bool is_paused() const { return !is_running(); }
		long long tick_count() const;

	private:
		// contains epoch when running; constains current time when paused
		long long epoch_;
	};

	struct scoped_timer_starter
	{
		scoped_timer_starter( timer& t ) : t_( t ) { t_.resume(); }
		~scoped_timer_starter() { t_.pause(); }
		timer& t_;
	};
}
