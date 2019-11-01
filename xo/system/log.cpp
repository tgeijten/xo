#include "log.h"
#include "log_sink.h"
#include "xo/system/assert.h"
#include "xo/numerical/math.h"
#include "xo/container/vector_type.h"
#include "xo/container/container_tools.h"
#include <stdarg.h>

namespace xo
{
	namespace log
	{
		level global_log_level = level::never_log_level; // lowest log level of all sinks
		xo::vector< sink* > global_sinks;

		void log_string( level l, const string& str )
		{
			// no need to do additional test_log_level(), no performance gain
			for ( auto s : global_sinks )
				s->try_send_log_message( l, str );
		}

		void log_vstring( level l, const char* format, va_list list )
		{
			if ( test_log_level( l ) ) // check for global log level first
			{
				char buf[ 1024 ];
				vsnprintf( buf, sizeof( buf ), format, list );
				log_string( l, string( buf ) ); // #todo: use string_view
			}
		}

		void flush()
		{
			for ( auto s : global_sinks )
				s->flush();
		}

		void add_sink( sink* s )
		{
			xo_assert( s != nullptr );
			if ( xo::find( global_sinks, s ) == global_sinks.end() )
			{
				global_sinks.push_back( s );
				update_global_log_level();
			}
		}

		void remove_sink( sink* s )
		{
			auto it = xo::find( global_sinks, s );
			if ( it != global_sinks.end() )
				global_sinks.erase( it );
			update_global_log_level();
		}

		void set_global_log_level( level l )
		{
			for ( auto s : global_sinks )
				s->set_log_level( l );
			global_log_level = l;
		}

		void update_global_log_level()
		{
			global_log_level = never_log_level;
			for ( auto s : global_sinks )
				global_log_level = xo::min( global_log_level, s->get_log_level() );
		}

		level get_global_log_level()
		{
			return global_log_level;
		}

		bool test_log_level( level l )
		{
			return l >= global_log_level;
		}

		void messagef( level l, const char* format, ... )
		{
			va_list va; va_start( va, format ); log_vstring( l, format, va ); va_end( va );
		}

		void tracef( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= trace_level ) {
				va_list va; va_start( va, format ); log_vstring( trace_level, format, va ); va_end( va );
			}
		}

		void debugf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= debug_level ) {
				va_list va; va_start( va, format ); log_vstring( debug_level, format, va ); va_end( va );
			}
		}

		void infof( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= info_level ) {
				va_list va; va_start( va, format ); log_vstring( info_level, format, va ); va_end( va );
			}
		}

		void warningf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= warning_level ) {
				va_list va; va_start( va, format ); log_vstring( warning_level, format, va ); va_end( va );
			}
		}

		void errorf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= error_level ) {
				va_list va; va_start( va, format ); log_vstring( error_level, format, va ); va_end( va );
				flush();
			}
		}

		void criticalf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= critical_level ) {
				va_list va; va_start( va, format ); log_vstring( critical_level, format, va ); va_end( va );
				flush();
			}
		}
	}
}
