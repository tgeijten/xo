#pragma once

#include "vec3.h"

namespace xo
{
	template< typename T >
	vec3_<T> closest_point_line( const vec3_<T>& p1, const vec3_<T>& p2, const vec3_<T>& q2 ) {
		auto ld = q2 - p2;
		auto t = dot_product( p1 - p2, ld ) / squared_length( ld );
		return p2 + t * ld;
	}
	
	template< typename T >
	vec3_<T> closest_point_segment( const vec3_<T>& p1, const vec3_<T>& p2, const vec3_<T>& q2 ) {
		auto ld = q2 - p2;
		auto t = clamped( dot_product( p1 - p2, ld ) / squared_length( ld ), T( 0 ), T( 1 ) );
		return p2 + t * ld;
	}
}
