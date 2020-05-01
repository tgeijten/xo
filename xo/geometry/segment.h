#pragma once

#include "vec3.h"
#include "xo/container/pair_type.h"
#include "xo/numerical/constants.h"

namespace xo
{
	/// struct representing a 3D line
	template< typename T >
	struct segment_
	{
		segment_() = default;
		segment_( const vec3_<T>& p0, const vec3_<T>& p1 ) : p0( p0 ), p1( p1 ) {}

		vec3_<T> p0;
		vec3_<T> p1;
	};

	using segmentf = segment_< float >;
	using segmentd = segment_< double >;

	template< typename T >
	T distance( const vec3_<T>& point, const segment_<T>& segment )
	{
		auto v = segment.p1 - segment.p0;
		auto w = point - segment.p0;
		auto c1 = dot_product( w, v );
		if ( c1 <= 0 )
			return distance( point, segment.p0 );
		auto c2 = dot_product( v, v );
		if ( c2 <= c1 )
			return distance( point, segment.p1 );
		auto b = c1 / c2;
		auto pb = segment.p0 + b * v;
		return distance( point, pb );
	}
}
