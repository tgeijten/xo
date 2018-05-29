#pragma once

#include "xo/system/build_config.h"

namespace xo
{
	typedef double real_t;
	typedef real_t seconds_t;
	typedef long long nanoseconds_t;
	typedef long long microseconds_t;
	typedef long long milliseconds_t;

	typedef size_t index_t;
	const size_t no_size = ~size_t( 0 );
	const index_t no_index = ~index_t( 0 );
}
