#pragma once

#include "xo/system/xo_config.h"
#include "xo/time/time.h"

namespace xo
{
	struct XO_API timer
	{
		timer( bool start = true );
		timer( const timer& ) = delete;
		time operator()() const;

		time pause();
		void resume();
		time restart();

		bool is_running() const { return epoch_ > 0; }

	private:
		// contains epoch when running; constains current time when paused
		long long epoch_;
	};

	struct scope_timer
	{
		scope_timer( timer& t ) : t_( t ) { t_.resume(); }
		~scope_timer() { t_.pause(); }
		timer& t_;
	};
}
