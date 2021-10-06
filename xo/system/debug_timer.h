#pragma once

#include "xo/system/log.h"
#include "xo/time/timer.h"

#define XO_FUNCTION_TIMER ::xo::debug_timer _function_debug_timer_( __FUNCTION__ )

namespace xo
{
	struct debug_timer
	{
		debug_timer( const char* name, log::level l = log::level::info ) :
			name_( name ),
			level_( l ),
			timer_( true )
		{}
		~debug_timer() {
			auto t = timer_();
			log::message( level_, name_, " took ", t.milliseconds(), "ms" );
		}

		const char* name_;
		log::level level_;
		timer timer_;
	};
		
}
