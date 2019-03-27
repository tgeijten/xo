#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"
#include "xo/system/log_level.h"
#include "xo/string/string_cast.h"


#define xo_logvar( var_ ) xo::log::trace( #var_"=", var_ )
#define xo_logvar2( var1_, var2_ ) xo::log::trace( #var1_"=", var1_, "\t", #var2_"=", var2_ )
#define xo_logvar3( var1_, var2_, var3_ ) xo::log::trace( #var1_"=", var1_, "\t", #var2_"=", var2_, "\t", #var3_"=", var3_ )
#define xo_logvar4( var1_, var2_, var3_, var4_ ) xo::log::trace( #var1_"=", var1_, "\t", #var2_"=", var2_, "\t", #var3_"=", var3_, "\t", #var4_"=", var4_ )

#define xo_do_periodic( interval_, statement_ ) { static local_count_ = 0; if ( local_count_++ % interval_ == 0 ) { statement_; } }
#define xo_trace_call( function_ ) xo::log::trace( "--> "#function_ ); function_; xo::log::trace( "<-- "#function_ )

namespace xo
{
	namespace log
	{
		// output sink
		class sink;
		XO_API void add_sink( sink* s );
		XO_API void remove_sink( sink* s );

		// dynamic log level
		XO_API void set_global_log_level( level l );
		XO_API level get_global_log_level();
		XO_API bool test_log_level( level l );

		// log independent of level
		XO_API void log_string( level l, const string& s );
		XO_API void log_vstring( level l, const char* format, va_list list );

		template< typename T, typename... Args > void log_string( level l, std::string& s, T v, const Args&... args ) {
			s += to_str( v );
			log_string( l, s, args... );
		}

		// log at specified level
		template< typename... Args > void message( level l, const Args&... args ) {
			if ( test_log_level( l ) ) {
				std::string s;
				s.reserve( 256 );
				log_string( l, s, args... );
			}
		}
		void messagef( level l, const char* format, ... );

		template< typename... Args > void trace( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= trace_level )
				message( trace_level, args... );
		}
		XO_API void tracef( const char* format, ... );

		template< typename... Args > void debug( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= debug_level )
				message( debug_level, args... );
		}
		XO_API void debugf( const char* format, ... );

		template< typename... Args > void info( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= info_level )
				message( info_level, args... );
		}
		XO_API void infof( const char* format, ... );

		template< typename... Args > void warning( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= warning_level )
				message( warning_level, args... );
		}
		XO_API void warningf( const char* format, ... );

		template< typename... Args > void error( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= error_level )
				message( error_level, args... );
		}
		XO_API void errorf( const char* format, ... );

		template< typename... Args > void critical( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= critical_level )
				message( critical_level, args... );
		}
		XO_API void criticalf( const char* format, ... );
	}
}
