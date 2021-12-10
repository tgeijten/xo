#pragma once

#include "xo/system/log.h"
#include "xo/time/timer.h"
#include "xo/string/string_tools.h"

#define XO_FUNCTION_TIMER ::xo::scope_timer _function_debug_timer_( __FUNCTION__ )

namespace xo
{
	struct scope_timer
	{
		scope_timer( const char* name, log::level l = log::level::info ) :
			name_( name ),
			level_( l ),
			timer_( true )
		{}
		~scope_timer() {
			auto t = timer_();
			log::message( level_, name_, " took ", stringf( "%3.2fms", t.milliseconds() ) );
		}

		const char* name_;
		log::level level_;
		timer timer_;
	};
}
