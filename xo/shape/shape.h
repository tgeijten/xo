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

	XO_API string to_str( const sphere& s );
	XO_API string to_str( const box& s );
	XO_API string to_str( const cylinder& s );
	XO_API string to_str( const capsule& s );
	XO_API string to_str( const cone& s );
	XO_API string to_str( const plane& s );

	inline float volume( const shape& s ) {
		return std::visit( []( auto&& arg ) { return volume( arg ); }, s );
	}

	inline vec3f dim( const shape& s ) {
		return std::visit( []( auto&& arg ) { return dim( arg ); }, s );
	}

	inline aabbf aabb( const shape& s, const transformf& t ) {
		return std::visit( [&]( auto&& sarg ) { return aabb( sarg, t ); }, s );
	}

	inline vec3f inertia( const shape& s, float density ) {
		return std::visit( [&]( auto&& arg ) { return inertia( arg, density ); }, s );
	}

	inline void scale( shape& s, float f ) {
		std::visit( [&]( auto&& arg ) { scale( arg, f ); }, s );
	}

	inline prop_node to_prop_node( const shape& s ) {
		return std::visit( []( auto&& arg ) { return to_prop_node( arg ); }, s );
	}

	inline string to_str( const shape& s ) {
		return std::visit( []( auto&& arg ) { return to_str( arg ); }, s );
	}
}
