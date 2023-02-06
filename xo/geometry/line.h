#pragma once

#include "vec3.h"
#include "xo/container/pair_type.h"
#include "xo/numerical/constants.h"

namespace xo
{
	/// struct representing a 3D line
	template< typename T >
	struct line_
	{
		line_() = default;
		line_( const vec3_<T>& pos, const vec3_<T>& dir ) : pos( pos ), dir( dir ) {}

		vec3_<T> pos;
		vec3_<T> dir;
	};

	using linef = line_< float >;
	using lined = line_< double >;

	template< typename T >
	pair< vec3_<T>, vec3_<T> > closest_points( const line_<T>& l1, const line_<T>& l2 )
	{
		// based on http://geomalgorithms.com/a07-_distance.html
		auto w = l1.pos - l2.pos;
		auto a = dot_product( l1.dir, l1.dir );         // always >= 0
		auto b = dot_product( l1.dir, l2.dir );
		auto c = dot_product( l2.dir, l2.dir );         // always >= 0
		auto d = dot_product( l1.dir, w );
		auto e = dot_product( l2.dir, w );
		auto D = a * c - b * b;        // always >= 0

		T sc, tc;
		if ( D < constants<T>::ample_epsilon() ) {
			sc = 0.0;
			tc = ( b > c ? d / b : e / c );    // use the largest denominator
		}
		else {
			sc = ( b * e - c * d ) / D;
			tc = ( a * e - b * d ) / D;
		}

		// get the difference of the two closest points
		return { l1.pos + sc * l1.dir, l2.pos + tc * l2.dir };  // =  L1(sc) - L2(tc)
	}

	template< typename T >
	T distance( const line_<T>& l1, const line_<T>& l2 ) {
		auto p = closest_points( l1, l2 );
		return distance( p.first, p.second );
	}
}
