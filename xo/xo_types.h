#pragma once

#include "xo/system/xo_config.h"
#include <stddef.h>

namespace xo
{
	// forward declaration of common xo classes
	class prop_node;
	class path;
	struct error_code;
	class char_stream;

	template< typename T > struct vec3_;
	template< typename T > struct quat_;
	template< typename T > struct transform_;
	template< typename T > class bounds;

	// simple function type
	template< typename T > using simple_function_t = T( * )( T );


	// character defaults
	constexpr const char* whitespace_characters = " \t\r\n\f\v";

	// xo index type
	typedef size_t index_t;
	const index_t no_index = ~index_t( 0 );

	inline namespace literals {}
	namespace log {}
}
