#pragma once

#include "vec3.h"
#include "xo/container/pair_type.h"

namespace xo
{
	template< typename T >
	vec3_<T> closest_point_line( const vec3_<T>& p, const vec3_<T>& l1, const vec3_<T>& l2 ) {
		auto ld = l2 - l1;
		auto t = dot_product( p - l1, ld ) / squared_length( ld );
		return l1 + t * ld;
	}
	
	template< typename T >
	T distance_point_line( const vec3_<T>& p, const vec3_<T>& l1, const vec3_<T>& l2 ) {
		auto ld = l2 - l1;
		return length( cross_product( p - l1, ld ) ) / length( ld );
	}
	
	template< typename T >
	vec3_<T> closest_point_segment( const vec3_<T>& p, const vec3_<T>& s1, const vec3_<T>& s2 ) {
		auto ld = s2 - s1;
		auto t = clamped( dot_product( p - s1, ld ) / squared_length( ld ), T( 0 ), T( 1 ) );
		return s1 + t * ld;
	}

	template< typename T >
	pair<vec3_<T>, vec3_<T>> closest_line_line( const vec3_<T>& l1, const vec3_<T>& l2, const vec3_<T>& m1, const vec3_<T>& m2 ) {
		vec3_<T> ldir = l2 - l1;
		vec3_<T> mdir = m2 - m1;
		vec3_<T> l1m1 = l1 - m1;
		T ldot = squared_length( ldir );
		T mdot = squared_length( mdir );
		T lmdot = dot_product( ldir, mdir );
		T llmdot = dot_product( ldir, l1m1 );
		T mlmdot = dot_product( mdir, l1m1 );
		T denom = ldot * mdot - lmdot * lmdot;
		T tl = ( denom != T( 0 ) ) ? ( lmdot * mlmdot - llmdot * mdot ) / ( denom ) : T( 0 );
		T tm = ( lmdot * tl + mlmdot ) / mdot;
		return { l1 + ldir * tl, m1 + mdir * tm };
	}
}
