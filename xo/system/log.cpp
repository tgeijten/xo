#include "xo/system/assert.h"
#include "log.h"
#include "log_sink.h"

#include <stdarg.h>
#include <vector>

namespace xo
{
	namespace log
	{
		void std_cout_log( level l, const std::string& msg ) { std::cout << msg << std::endl; }
		level lowest_log_level = level::never_log_level;
		std::vector< sink* > global_sinks;

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
				log_string( l, string( buf ) ); // TODO: use string_view
			}
		}

		void add_sink( sink* s )
		{
			xo_assert( s != nullptr );
			if ( std::find( global_sinks.begin(), global_sinks.end(), s ) == global_sinks.end() )
			{
				global_sinks.push_back( s );
				get_global_log_level();
			}
		}

		void remove_sink( sink* s )
		{
			auto it = std::find( global_sinks.begin(), global_sinks.end(), s );
			if ( it != global_sinks.end() ) global_sinks.erase( it );
			get_global_log_level();
		}

		void set_global_log_level( level l )
		{
			for ( auto s : global_sinks )
				s->set_log_level( l );
			lowest_log_level = l;
		}

		level get_global_log_level()
		{
			lowest_log_level = critical_level;
			for ( auto s : global_sinks )
				if ( s->get_log_level() < lowest_log_level ) lowest_log_level = s->get_log_level();
			return lowest_log_level;
		}

		bool test_log_level( level l )
		{
			return l >= lowest_log_level;
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
			}
		}

		void criticalf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= critical_level ) {
				va_list va; va_start( va, format ); log_vstring( critical_level, format, va ); va_end( va );
			}
		}
	}
}
