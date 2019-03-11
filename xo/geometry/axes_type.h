#pragma once

#include "vec3_type.h"

namespace xo
{
	template< typename T >
	struct axes_
	{
		vec3_<T> x;
		vec3_<T> y;
		vec3_<T> z;
	};
}
