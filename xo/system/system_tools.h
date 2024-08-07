#pragma once

#include "xo/system/xo_config.h"
#include "xo/system/error_code.h"
#include "xo/system/version.h"
#include <typeinfo>
#include <ctime>

namespace xo
{
	XO_API version get_xo_version();
	XO_API char wait_for_key();
	XO_API std::time_t get_current_time();
	XO_API string get_date_time_str( const std::time_t& t, const char* format = "%Y-%m-%d_%H.%M.%S" );
	XO_API string get_date_time_str( const char* format = "%Y-%m-%d_%H.%M.%S" );
	XO_API void crash( const string& message = "" );
	XO_API void sleep( int milliseconds );
	XO_API string tidy_identifier( const string& id );
	XO_API string tidy_type_name( const string& name );
	XO_API string get_computer_name();
	XO_API string get_compiler_id();
	template< typename T > string get_type_name() { return string( typeid( T ).name() ); }
	template< typename T > string get_clean_type_name() { return tidy_type_name( get_type_name<T>() ); }
	template< typename T > string get_type_name( const T& obj ) { return string( typeid( obj ).name() ); }
	template< typename T > string get_clean_type_name( const T& obj ) { return tidy_type_name( get_type_name( obj ) ); }
}
