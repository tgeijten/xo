#pragma once

#include "xo/xo_types.h"
#include "xo/system/xo_config.h"
#include "xo/geometry/vec3.h"
#include "xo/container/array.h"

namespace xo
{
	struct box
	{
		box() : half_dim_() {}
		box( const vec3f& half_dim ) : half_dim_( half_dim ) {}

		vec3f half_dim_;
	};

	XO_API float volume( const box& s );
	XO_API aabb_<float> aabb( const box& s, const transform_<float>& t );
	XO_API vec3f dim( const box& s );
	XO_API vec3f inertia( const box& s, float density );
	XO_API void scale( box& s, float f );
	XO_API array< vec3f, 8 > corners( const box& b );
}
