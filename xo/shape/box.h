#pragma once

#include "xo/xo_types.h"
#include "xo/system/xo_config.h"
#include "xo/geometry/vec3_type.h"
#include "xo/container/array.h"

namespace xo
{
	struct box
	{
		box() : half_dim_() {}
		explicit box( const vec3f& half_dim ) : half_dim_( half_dim ) {}
		explicit box( float hx, float hy, float hz ) : half_dim_( hx, hy, hz ) {}

		vec3f half_dim_;
	};

	inline box make_box( float x_size, float y_size, float z_size ) {
		return box{ 0.5f * x_size, 0.5f * y_size, 0.5f * z_size };
	}

	XO_API float volume( const box& s );
	XO_API aabb_<float> aabb( const box& s, const transform_<float>& t );
	XO_API vec3f dim( const box& s );
	XO_API vec3f inertia( const box& s, float density );
	XO_API void scale( box& s, float f );
	XO_API void scale( box& s, const vec3f& sv );
	XO_API array< vec3f, 8 > corners( const box& b );
}
