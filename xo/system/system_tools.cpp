#include "system_tools.h"

#ifdef XO_COMP_MSVC
#	include <conio.h>
#	include <shlobj.h>
#	pragma warning( disable: 4996 )
#else
#   include <cxxabi.h>
#endif

#include "xo/string/string_tools.h"
#include "xo/string/string_cast_enum.h"
#include "xo/system/log.h"

#include <fstream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <sstream>
#include "assert.h"

namespace xo
{
	version XO_VERSION = version( 0, 1, 0 );
	version get_xo_version() { return XO_VERSION; }

	XO_API char wait_for_key()
	{
#ifdef XO_COMP_MSVC
			return _getch();
#else
			return 0;
#endif
	}

	XO_API string get_date_time_str( const char* format )
	{
		auto in_time_t = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
		std::stringstream ss;
		ss << std::put_time( std::localtime( &in_time_t ), format );
		return ss.str();
	}

	XO_API void crash( const string& message )
	{
		if ( !message.empty() )
			log::critical( message );

		// crash!
		*(volatile int*)(0) = 123;
	}

	XO_API void sleep( int ms )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
	}

	XO_API string tidy_identifier( const string& id )
	{
		size_t pos = id.find_last_of( ':' );
		if ( pos != string::npos )
			return trim_str( id.substr( pos + 1 ), "_" ); // remove anything before :: plus underscores
		else if ( str_begins_with( id, "m_" ) )
			return trim_right_str( id.substr( 2 ), "_" ); // remove m_ plus underscores
		else return trim_str( id, "_" ); // remove underscores
	}

	XO_API string tidy_type_name( string name )
	{
#ifndef XO_COMP_MSVC
		int status;
		char* cleanType = abi::__cxa_demangle( name.c_str(), 0, 0, &status );
		name = std::string( cleanType );
		free( cleanType );
#endif
		size_t pos = name.find_last_of( ": " );
		return ( pos != std::string::npos ) ? name.substr( pos + 1 ) : name;
	}

#ifdef XO_COMP_MSVC
	// helper function for setting priorities on windows
	void set_win_prio( DWORD priority_class, int priority )
	{
		auto h = ::GetCurrentThread();
		::SetPriorityClass( h, priority_class );
		::SetThreadPriority( h, priority );
	}
#endif

	XO_API void set_thread_priority( thread_priority p )
	{
#ifdef XO_COMP_MSVC
		switch (p)
		{
		case thread_priority::idle: // base priority = 1
			return set_win_prio( IDLE_PRIORITY_CLASS, THREAD_PRIORITY_IDLE );
		case thread_priority::lowest: // base priority = 4
			return set_win_prio( BELOW_NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_LOWEST );
		case thread_priority::low: // base priority = 6
			return set_win_prio( BELOW_NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_NORMAL );
		case thread_priority::normal: // base priority = 8
			return set_win_prio( NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_NORMAL );
		case thread_priority::high: // base priority = 11
			return set_win_prio( ABOVE_NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_ABOVE_NORMAL );
		case thread_priority::highest: // base priority = 15
			return set_win_prio( HIGH_PRIORITY_CLASS, THREAD_PRIORITY_HIGHEST );
		case thread_priority::realtime: // base priority = 25
			return set_win_prio( REALTIME_PRIORITY_CLASS, THREAD_PRIORITY_NORMAL );
		default: xo_error( "Unsupported thread priority: " + to_str( p ) );
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
}
