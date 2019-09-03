#pragma once

#include "xo/system/xo_config.h"
#include <cstddef>
#include <cstdint>

namespace xo
{
	// forward declaration of common xo classes / structs
	class prop_node;
	class path;
	struct error_code;
	class char_stream;
	struct version;
	struct time;

	// forward declaration of common xo template classes / structs
	template< typename T > struct vec3_;
	template< typename T > struct quat_;
	template< typename T > struct transform_;
	template< typename T > struct aabb_;
	template< typename T > class bounds;

	// simple function type
	template< typename T > using simple_function_t = T( * )( T );

	// character defaults
	constexpr const char* whitespace_characters = " \t\r\n\f\v";

	// numerical types
	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	// xo index type
	using index_t = size_t;
	const index_t no_index = ~index_t( 0 );

	inline namespace literals {}
	namespace log {}
}
