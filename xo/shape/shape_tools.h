#pragma once

#include "shape.h"
#include "xo/geometry/line.h"

namespace xo
{
	template< typename T >
	vec3_<T> intersection( const line_<T>& r, const plane& s, const transform_<T>& t ) {
		auto delta_pos = r.pos - t.p;
		auto plane_normal = t.q * vec3_<T>( s.normal_ );
		auto p1 = dot_product( delta_pos, plane_normal );
		auto p2 = dot_product( r.dir, plane_normal );
		return r.pos - ( p1 / p2 ) * r.dir;
	}

	float distance_from_center( xo::sphere& s, const vec3f& dir ) {
		return s.radius_;
	}
}
