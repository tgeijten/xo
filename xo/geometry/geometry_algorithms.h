#pragma once

#include "vec3.h"
#include "xo/container/pair_type.h"

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

	template< typename T >
	pair<vec3_<T>, vec3_<T>> closest_line_line( const vec3_<T>& p1, const vec3_<T>& p2, const vec3_<T>& q1, const vec3_<T>& q2 ) {
		vec3_<T> pdir = p2 - p1;
		vec3_<T> qdir = q2 - q1;
		vec3_<T> p1q1 = p1 - q1;
		T pdot = squared_length( pdir );
		T qdot = squared_length( qdir );
		T pqdot = dot_product( pdir, qdir );
		T ppqdot = dot_product( pdir, p1q1 );
		T qpqdot = dot_product( qdir, p1q1 );
		T denom = pdot * qdot - pqdot * pqdot;
		T pt = ( denom != T( 0 ) ) ? ( pqdot * qpqdot - ppqdot * qdot ) / ( denom ) : T( 0 );
		T qt = ( pqdot * pt + qpqdot ) / qdot;
		return { p1 + pdir * pt, q1 + qdir * qt };
	}
}
