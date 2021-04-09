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

	inline float distance_from_center( const xo::sphere& s, const vec3f& dir ) { return s.radius_; }
	inline float distance_from_center( const xo::box& s, const vec3f& dir ) {
		return xo::min( s.half_dim_.x / dir.x, xo::min( s.half_dim_.y / dir.y, s.half_dim_.z / dir.z ) );
	}
	inline float distance_from_center( const xo::cylinder& s, const vec3f& dir ) { XO_NOT_IMPLEMENTED; }
	inline float distance_from_center( const xo::capsule& s, const vec3f& dir ) { XO_NOT_IMPLEMENTED; }
	inline float distance_from_center( const xo::cone& s, const vec3f& dir ) { XO_NOT_IMPLEMENTED; }
	inline float distance_from_center( const xo::plane& s, const vec3f& dir ) { return 0.0f; }
	inline float distance_from_center( const shape& s, const vec3f& dir ) {
		return std::visit( [&]( auto&& arg ) { return distance_from_center( arg, dir ); }, s );
	}

	inline vec3f intersection_from_center( const shape& s, const vec3f& dir ) {
		return distance_from_center( s, dir ) * dir;
	}
}
