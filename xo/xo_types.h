#pragma once

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

	// character defaults
	constexpr const char* whitespace_characters = " \t\r\n\f\v";

	// TODO: move these types elsewhere (and perhaps use std::chrono)
	using seconds_t = double;
	using nanoseconds_t = long long;
	using microseconds_t = long long;
	using milliseconds_t = long long;

	// xo index type
	typedef size_t index_t;
	const index_t no_index = ~index_t( 0 );
}
