#pragma once

#include "xo/geometry/vec3_type.h"

namespace xo
{
	struct plane
	{
		vec3f normal_;
	};

	inline float volume( const plane& s ) { return 0.0; }
	inline aabbf aabb( const plane& s, const transformf& t ) { XO_NOT_IMPLEMENTED; }
	inline vec3f dim( const plane& s ) { XO_NOT_IMPLEMENTED; }
	inline vec3f inertia( const plane& s, float density ) { return vec3f::zero(); }
	inline void scale( plane& s, float f ) { }
}
