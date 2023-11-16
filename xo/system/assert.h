#pragma once

#include "xo/system/xo_config.h"
#include <string>

#if XO_EXCEPTIONS_ENABLED
#	include <stdexcept>
#	define xo_error( MESSAGE_ ) \
		throw std::runtime_error( MESSAGE_ )
#else
#	include "xo/system/log.h"
#	define xo_error( MESSAGE_ ) \
		{ xo::log::critical( MESSAGE_ ); exit( -1 ); }
#endif

#if XO_ASSERT_ENABLED
#	define xo_assert( EXPRESSION_ ) \
		if (!(EXPRESSION_)) xo_error( "Assertion failure in " + std::string( __FUNCTION__ ) + "(): "#EXPRESSION_ )
#	define xo_assert_msg( EXPRESSION_, MESSAGE_ ) \
		if (!(EXPRESSION_)) xo_error( "Assertion failure in " + std::string( __FUNCTION__ ) + "(): "#EXPRESSION_" (" + std::string( MESSAGE_ ) + ")" )
#	if XO_IS_DEBUG_BUILD
#		define xo_debug_assert( EXPRESSION_ ) \
			if ( !( EXPRESSION_ ) ) xo_error( "Debug assertion failure in " + std::string( __FUNCTION__ ) + "(): "#EXPRESSION_ )
#	else // XO_IS_DEBUG_BUILD
#		define xo_debug_assert( EXPRESSION_ )
#	endif
#else // XO_ASSERT_ENABLED
#	define xo_assert( EXPRESSION_ )
#	define xo_assert_msg( EXPRESSION_, MESSAGE_ )
#endif

#define xo_error_if( CONDITION_, MESSAGE_ ) \
	if ( CONDITION_ ) xo_error( std::string( MESSAGE_ ) )

#define xo_range_assert( VAR_, VAL_MIN_, VAL_MAX_ ) \
	if ( VAR_ < VAL_MIN_ || VAR_ > VAL_MAX_ ) xo_error( "Variable out of range: "#VAR_ )

#define XO_NOT_IMPLEMENTED \
	xo_error( std::string( __FUNCTION__ ) + "(): Function not implemented" );
