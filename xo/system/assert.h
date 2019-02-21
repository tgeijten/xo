#pragma once

#include "xo/system/build_config.h"
#include <string>

#if XO_USE_EXCEPTIONS
#	include <stdexcept>
#	define xo_error( message_ ) \
		throw std::runtime_error( message_ )
#else
#	define xo_error( message_ ) \
		{ std::cout << message_ << std::endl; exit( -1 ) }
#endif

#if XO_USE_ASSERT
#	define xo_assert( expression_ ) \
		if (!(expression_)) xo_error( "Assertion Failure in " + std::string( __FUNCTION__ ) + "(): "#expression_ )

#	define xo_assert_msg( expression_, message_ ) \
		if (!(expression_)) xo_error( "Assertion Failure in " + std::string( __FUNCTION__ ) + "(): "#expression_" (" + std::string( message_ ) + ")" )
#else
#	define xo_assert( expression_ )
#	define xo_debug_assert( expression_ )
#	define xo_assert_msg( expression_, message_ )
#endif

#ifndef NDEBUG
#	define xo_debug_assert( expression_ ) xo_assert( expression_ )
#else
#	define xo_debug_assert( expression_ )
#endif

#define xo_error_if( condition_, message_ ) \
	if ( condition_ ) xo_error( std::string( message_ ) )

#define XO_NOT_IMPLEMENTED \
	xo_error( std::string( __FUNCTION__ ) + "(): Function not implemented" );
