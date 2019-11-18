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

	XO_API bool from_prop_node( const prop_node& pn, sphere& s );
	XO_API bool from_prop_node( const prop_node& pn, box& s );
	XO_API bool from_prop_node( const prop_node& pn, cylinder& s );
	XO_API bool from_prop_node( const prop_node& pn, capsule& s );
	XO_API bool from_prop_node( const prop_node& pn, cone& s );
	XO_API bool from_prop_node( const prop_node& pn, plane& s );
	XO_API bool from_prop_node( const prop_node& pn, shape& s );

	XO_API prop_node to_prop_node( const sphere& s );
	XO_API prop_node to_prop_node( const box& s );
	XO_API prop_node to_prop_node( const cylinder& s );
	XO_API prop_node to_prop_node( const capsule& s );
	XO_API prop_node to_prop_node( const cone& s );
	XO_API prop_node to_prop_node( const plane& s );
	XO_API prop_node to_prop_node( const shape& s );

	XO_API float volume( const shape& s );
	XO_API vec3f dim( const shape& s );
	XO_API aabbf aabb( const shape& s, const transformf& t );
	XO_API vec3f inertia( const shape& s, float density );

	XO_API void scale( shape& s, float f );
}
