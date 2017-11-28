#include "xo/system/assert.h"
#include "log.h"
#include "log_sink.h"
#include <stdarg.h>

namespace xo
{
	namespace log
	{
		void std_cout_log( level l, const std::string& msg ) { std::cout << msg << std::endl; }
		level lowest_log_level = level::never_log_level;
		std::vector< sink* > global_sinks;

		void log_string( level l, const string& str )
		{
			for ( auto s : global_sinks )
				s->try_send_log_message( l, str );
		}

		void log_vstring( level l, const char* format, va_list list )
		{
			if ( test_log_level( l ) )
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
}
}
