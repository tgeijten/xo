#include "thread_priority.h"

#ifdef XO_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#else
#   include <cxxabi.h>
#endif

#include "xo/system/assert.h"

namespace xo
{
	XO_API void set_thread_priority( thread_priority p )
	{
#ifdef XO_COMP_MSVC
		switch (p)
		{
		case thread_priority::idle: // base priority = 1
			return set_thread_priority( { IDLE_PRIORITY_CLASS, THREAD_PRIORITY_IDLE } );
		case thread_priority::lowest: // base priority = 4
			return set_thread_priority( { BELOW_NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_LOWEST } );
		case thread_priority::low: // base priority = 6
			return set_thread_priority( { BELOW_NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_NORMAL } );
		case thread_priority::normal: // base priority = 8
			return set_thread_priority( { NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_NORMAL } );
		case thread_priority::high: // base priority = 11
			return set_thread_priority( { ABOVE_NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_ABOVE_NORMAL } );
		case thread_priority::highest: // base priority = 15
			return set_thread_priority( { HIGH_PRIORITY_CLASS, THREAD_PRIORITY_HIGHEST } );
		case thread_priority::realtime: // base priority = 25
			return set_thread_priority( { REALTIME_PRIORITY_CLASS, THREAD_PRIORITY_NORMAL } );
		default: xo_error( "Unsupported thread priority" );
		}
#else
		/*
		sched_param sch_params;
		int policy;
		if (pthread_getschedparam(pthread_self(), &policy, &sch_params)) {
			log::warning("Failed to set thread priority.");
			return;
		}
		switch (p)
		{
			// http://man7.org/linux/man-pages/man2/setpriority.2.html
			case thread_priority::idle:
				sch_params.sched_priority = 19; break;
			case thread_priority::lowest:
				sch_params.sched_priority = 18; break;
			case thread_priority::low:
				sch_params.sched_priority = 10; break;
			case thread_priority::normal:
				sch_params.sched_priority = 5; break;
			case thread_priority::high:
				sch_params.sched_priority = -5; break;
			case thread_priority::highest:
				sch_params.sched_priority = -19; break;
			case thread_priority::realtime:
				sch_params.sched_priority = -20; break;
			default: xo_error( "Unsupported thread priority: " + to_str( p ) );
		}
		if (pthread_setschedparam(pthread_self(), policy, &sch_params)) {
		    log::warning("Failed to set thread priority.");
		}
		*/
#endif
	}

	void set_thread_priority( thread_priority_data p )
	{
#ifdef XO_COMP_MSVC
		auto h = ::GetCurrentThread();
		::SetPriorityClass( h, p.pc_ );
		::SetThreadPriority( h, p.tp_ );
#endif
	}

	XO_API thread_priority_data get_thread_priority()
	{
#ifdef XO_COMP_MSVC
		thread_priority_data result;
		auto h = ::GetCurrentThread();
		result.pc_ = ::GetPriorityClass( h );
		result.tp_= ::GetThreadPriority( h );
		return result;
#else
		XO_NOT_IMPLEMENTED;
#endif
	}
}
