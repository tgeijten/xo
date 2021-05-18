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
		auto dist = vec3f{ s.half_dim_.x / std::abs( dir.x ), s.half_dim_.y / std::abs( dir.y ), s.half_dim_.z / std::abs( dir.z ) };
		return xo::min( dist.x, xo::min( dist.y, dist.z ) );
	}
	inline float distance_from_center( const xo::cylinder& s, const vec3f& dir ) {
		const auto hh = 0.5f * s.height_;
		const auto r = s.radius_;
		auto vxz = std::sqrt( dir.x * dir.x + dir.z * dir.z );
		if ( std::abs( dir.y ) * r > hh * vxz ) 
			return std::sqrt( hh * hh + vxz * vxz ); // point is on cylinder top
		else return r / vxz; // point is on cylinder side
	}
	inline float distance_from_center( const xo::capsule& s, const vec3f& dir ) {
		const auto hh = 0.5f * s.height_;
		const auto r = s.radius_;
		auto vxz = std::sqrt( dir.x * dir.x + dir.z * dir.z );
		if ( std::abs( dir.y ) * r > hh * vxz ) { // point is on capsule cap
			auto b = 2.0f * dir.y * -std::copysign( hh, dir.y );
			auto c = hh * hh - r * r;
			auto det = b * b - 4 * c;
			auto sd = std::sqrt( det );
			return std::max( (-b + sd ) / 2, (-b - sd ) / 2 );
		}
		else return r / vxz; // point is on capsule cylinder
	}
	inline float distance_from_center( const xo::cone& s, const vec3f& dir ) { XO_NOT_IMPLEMENTED; }
	inline float distance_from_center( const xo::plane& s, const vec3f& dir ) { return 0.0f; }
	inline float distance_from_center( const shape& s, const vec3f& dir ) {
		return std::visit( [&]( auto&& arg ) { return distance_from_center( arg, dir ); }, s );
	}

	inline vec3f intersection_from_center( const shape& s, const vec3f& dir ) {
		return distance_from_center( s, dir ) * dir;
	}
}
