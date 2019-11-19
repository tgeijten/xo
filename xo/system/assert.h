#pragma once

#include "xo/system/xo_config.h"
#include <string>

#if XO_USE_EXCEPTIONS
#	include <stdexcept>
#	define xo_error( message_ ) \
		throw std::runtime_error( message_ )
#else
#	include "xo/system/log.h"
#	define xo_error( message_ ) \
		{ xo::log::critical( message_ ); exit( -1 ); }
#endif

#if XO_USE_ASSERT
#	define xo_assert( expression_ ) \
		if (!(expression_)) xo_error( "Assertion failure in " + std::string( __FUNCTION__ ) + "(): "#expression_ )
#	define xo_assert_msg( expression_, message_ ) \
		if (!(expression_)) xo_error( "Assertion failure in " + std::string( __FUNCTION__ ) + "(): "#expression_" (" + std::string( message_ ) + ")" )
#	if XO_IS_DEBUG_BUILD
#		define xo_debug_assert( expression_ ) \
			if ( !( expression_ ) ) xo_error( "Debug assertion failure in " + std::string( __FUNCTION__ ) + "(): "#expression_ )
#	else // XO_IS_DEBUG_BUILD
#		define xo_debug_assert( expression_ )
#	endif
#else // XO_USE_ASSERT
#	define xo_assert( expression_ )
#	define xo_assert_msg( expression_, message_ )
#endif

#define xo_error_if( condition_, message_ ) \
	if ( condition_ ) xo_error( std::string( message_ ) )

#define XO_NOT_IMPLEMENTED \
	xo_error( std::string( __FUNCTION__ ) + "(): Function not implemented" );
