#pragma once

#include "xo/xo_types.h"
#include "sphere.h"
#include "box.h"
#include "cylinder.h"
#include "capsule.h"
#include "cone.h"
#include "plane.h"
#include "aabb.h"

#include <variant>

namespace xo
{
	using shape = std::variant< sphere, box, cylinder, capsule, cone, plane >;

	XO_API bool from_prop_node( const prop_node& pn, shape& s );

	XO_API float volume( const shape& s );
	XO_API vec3f dim( const shape& s );
	XO_API aabbf aabb( const shape& s, const transformf& t );
	XO_API vec3f inertia( const shape& s, float density );

	XO_API void scale( shape& s, float f );
}
