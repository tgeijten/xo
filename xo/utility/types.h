#pragma once

#include <stddef.h>

namespace xo
{
	// TODO: move these types elsewhere (and perhaps use std::chrono)
	using seconds_t = double;
	using nanoseconds_t = long long;
	using microseconds_t = long long;
	using milliseconds_t = long long;

	typedef size_t index_t;
	const size_t no_size = ~size_t( 0 );
	const index_t no_index = ~index_t( 0 );
}
