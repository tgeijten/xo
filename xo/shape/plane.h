#pragma once

#include "xo/system/xo_config.h"
#include "xo/geometry/vec3_type.h"
#include "xo/geometry/ray.h"
#include "xo/geometry/transform.h"
#include "aabb.h"

namespace xo
{
	struct plane
	{
		plane() : normal_() {}
		plane( const vec3f& normal ) : normal_( normal ) {}

		vec3f normal_;
	};

	inline float volume( const plane& s ) { return 0.0; }
	inline aabbf aabb( const plane& s, const transformf& t ) { XO_NOT_IMPLEMENTED; }
	inline vec3f dim( const plane& s ) { XO_NOT_IMPLEMENTED; }
	inline vec3f inertia( const plane& s, float density ) { return vec3f::zero(); }
	inline void scale( plane& s, float f ) { }
	XO_API vec3f intersection( const rayf& r, const plane& s, const transformf& t );
}
