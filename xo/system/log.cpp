#include "log.h"
#include "log_sink.h"
#include "xo/system/assert.h"
#include "xo/numerical/math.h"
#include "xo/container/vector_type.h"
#include "xo/container/container_tools.h"
#include <stdarg.h>
#include <mutex>
#include <shared_mutex>

namespace xo
{
	namespace log
	{
		xo::vector< sink* > global_sinks;
		std::shared_mutex global_sink_mutex;
		level global_lowest_log_level = level::never;

		void log_string( level l, const string& str )
		{
			// no need to do additional test_log_level(), no performance gain
			std::shared_lock lock{ global_sink_mutex };
			for ( auto s : global_sinks )
				s->submit_log_message( l, str );
		}

		void log_vstring( level l, const char* format, va_list list )
		{
			if ( test_log_level( l ) ) // check for global log level first
			{
				char buf[1024];
				vsnprintf( buf, sizeof( buf ), format, list );
				log_string( l, string( buf ) ); // #todo: use string_view
			}
		}

		void flush()
		{
			std::shared_lock lock{ global_sink_mutex };
			for ( auto s : global_sinks )
				s->flush();
		}

		void update_global_log_level_thread_unsafe()
		{
			global_lowest_log_level = log::level::never;
			for ( auto s : global_sinks )
				xo::set_if_smaller( global_lowest_log_level, s->get_log_level() );
		}

		void add_sink( sink* s )
		{
			xo_assert( s != nullptr );
			std::unique_lock lock{ global_sink_mutex };
			if ( xo::find( global_sinks, s ) == global_sinks.end() )
				global_sinks.push_back( s );
			update_global_log_level_thread_unsafe();
		}

		void remove_sink( sink* s )
		{
			std::unique_lock lock{ global_sink_mutex };
			auto it = xo::find( global_sinks, s );
			if ( it != global_sinks.end() )
				global_sinks.erase( it );
			update_global_log_level_thread_unsafe();
		}

		bool test_log_level( level l )
		{
			if ( l < global_lowest_log_level )
				return false;

			std::shared_lock lock{ global_sink_mutex };
			for ( auto s : global_sinks )
				if ( s->test_log_level( l ) )
					return true;
			return false;
		}

		void messagef( level l, const char* format, ... )
		{
			va_list va; va_start( va, format ); log_vstring( l, format, va ); va_end( va );
		}

		void tracef( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::trace ) {
				va_list va; va_start( va, format ); log_vstring( level::trace, format, va ); va_end( va );
			}
		}

		void debugf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::debug ) {
				va_list va; va_start( va, format ); log_vstring( level::debug, format, va ); va_end( va );
			}
		}

		void infof( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::info ) {
				va_list va; va_start( va, format ); log_vstring( level::info, format, va ); va_end( va );
			}
		}

		void warningf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::warning ) {
				va_list va; va_start( va, format ); log_vstring( level::warning, format, va ); va_end( va );
			}
		}

		void errorf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::error ) {
				va_list va; va_start( va, format ); log_vstring( level::error, format, va ); va_end( va );
				flush();
			}
		}

		void criticalf( const char* format, ... )
		{
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::critical ) {
				va_list va; va_start( va, format ); log_vstring( level::critical, format, va ); va_end( va );
				flush();
			}
		}
	}
}
