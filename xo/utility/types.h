#pragma once

#include "xo/system/build_config.h"

namespace xo
{
	// TODO: do we really need this?
	using real_t = double;

	// TODO: move these types elsewhere (and perhaps use std::chrono)
	using seconds_t = real_t;
	using nanoseconds_t = long long;
	using microseconds_t = long long;
	using milliseconds_t = long long;

	typedef size_t index_t;
	const size_t no_size = ~size_t( 0 );
	const index_t no_index = ~index_t( 0 );
}
