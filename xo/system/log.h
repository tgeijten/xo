#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"
#include "xo/string/string_cast.h"
#include "xo/system/log_level.h"

#define xo_logvar( var_ ) xo::log::debug( #var_"=", var_ )
#define xo_logvar2( var1_, var2_ ) xo::log::debug( #var1_"=", var1_, "\t", #var2_"=", var2_ )
#define xo_logvar3( var1_, var2_, var3_ ) xo::log::debug( #var1_"=", var1_, "\t", #var2_"=", var2_, "\t", #var3_"=", var3_ )
#define xo_logvar4( var1_, var2_, var3_, var4_ ) xo::log::debug( #var1_"=", var1_, "\t", #var2_"=", var2_, "\t", #var3_"=", var3_, "\t", #var4_"=", var4_ )
#define xo_logvar5( var1_, var2_, var3_, var4_, var5_ ) xo::log::debug( #var1_"=", var1_, "\t", #var2_"=", var2_, "\t", #var3_"=", var3_, "\t", #var4_"=", var4_, "\t", #var5_"=", var5_ )

#define xo_trace_call( function_ ) xo::log::trace( "--> "#function_ ); function_; xo::log::trace( "<-- "#function_ )
#define xo_log_if_different( var1_, var2_ ) if ( var1_ != var2_ ) xo::log::debug( #var1_"=", var1_, "\t", #var2_"=", var2_ )

namespace xo
{
	namespace log
	{
		// output sink
		class sink;
		XO_API void add_sink( sink* s );
		XO_API void remove_sink( sink* s );

		// test log level for all sinks
		XO_API bool test_log_level( level l );

		// log with specific level
		XO_API void log_string( level l, const string& s );
		XO_API void log_vstring( level l, const char* format, va_list list );

		// flush all sinks, happens automatically if level >= level::error
		XO_API void flush();

		template< typename T, typename... Args > void log_string( level l, std::string& s, const T& v, const Args&... args ) {
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
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::trace )
				message( level::trace, args... );
		}
		XO_API void tracef( const char* format, ... );

		template< typename... Args > void debug( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::debug )
				message( level::debug, args... );
		}
		XO_API void debugf( const char* format, ... );

		template< typename... Args > void info( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::info )
				message( level::info, args... );
		}
		XO_API void infof( const char* format, ... );

		template< typename... Args > void warning( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::warning )
				message( level::warning, args... );
		}
		XO_API void warningf( const char* format, ... );

		template< typename... Args > void error( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::error ) {
				message( level::error, args... );
				flush();
			}
		}
		XO_API void errorf( const char* format, ... );

		template< typename... Args > void critical( const Args&... args ) {
			if constexpr ( XO_STATIC_LOG_LEVEL <= level::critical ) {
				message( level::critical, args... );
				flush();
			}
		}
		XO_API void criticalf( const char* format, ... );
	}
}
