#pragma once

#include "xo/system/xo_config.h"
#ifndef XO_COMP_MSVC
#	include "xo/system/log.h"
#endif

namespace xo
{
	enum class thread_priority { idle, lowest, low, normal, high, highest, realtime };
	XO_API void set_thread_priority( thread_priority p );

	/// platform-specific thread priority data
	struct thread_priority_data {
#ifdef XO_COMP_MSVC
		unsigned long pc_;
		int tp_;
#else
		int sp_;
#endif
	};

	XO_API void set_thread_priority( thread_priority_data p );
	XO_API thread_priority_data get_thread_priority();

#ifdef XO_COMP_MSVC
	struct XO_API scoped_thread_priority {
		scoped_thread_priority( thread_priority p ) : tpimpl_( get_thread_priority() ) { set_thread_priority( p ); }
		~scoped_thread_priority() { set_thread_priority( tpimpl_ ); }
		thread_priority_data tpimpl_;
	};
#else
	struct scoped_thread_priority {
		scoped_thread_priority( thread_priority p ) { log::warning( "get_thread_priority() is not implemented for this platform" ); }
};
#endif
}
