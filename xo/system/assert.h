#pragma once

#include "xo/system/build_config.h"
#include <cstdlib>

#if XO_USE_EXCEPTIONS
#	include <stdexcept>
	namespace xo
	{
		struct exception : public std::runtime_error { exception( const std::string& msg ) : runtime_error( msg ) {} };
	}
	#define XO_EXCEPTION( message_ ) throw xo::exception( message_ )
#else
	#define XO_EXCEPTION( message_ ) std::cout << message_ << std::endl; exit( -1 )
#endif

#if XO_USE_ASSERT
#define xo_assert( expression_ ) \
	if (!(expression_)) XO_EXCEPTION( "Assertion Failure in " + std::string( __FUNCTION__ ) + "(): "#expression_ )

	#define xo_assert_msg( expression_, message_ ) \
	if (!(expression_)) XO_EXCEPTION( "Assertion Failure in " + std::string( __FUNCTION__ ) + "(): "#expression_" (" + std::string( message_ ) + ")" )
#else
	#define xo_assert( expression_ )
	#define xo_assert_msg( expression_, message_ )
#endif

/// throw exception
#define xo_error( message_ ) \
	XO_EXCEPTION( std::string( message_ ) )

/// conditional throw exception
#define xo_error_if( condition_, message_ ) \
	if ( condition_ ) xo_error( std::string( message_ ) )

/// throw exception
#define xo_validate( expression_ ) \
	if (!(expression_)) XO_EXCEPTION( "Validation Failure in " + std::string( __FUNCTION__ ) + "(): "#expression_ )

#define xo_validate_msg( expression_, message_ ) \
	if (!(expression_)) XO_EXCEPTION( "Validation Failure in " + std::string( __FUNCTION__ ) + "(): "#expression_" (" + std::string( message_ ) + ")" )

/// not implemented exception
#define XO_NOT_IMPLEMENTED XO_EXCEPTION( std::string( __FUNCTION__ ) + "(): Function not implemented" );
