#pragma once

#include "xo/geometry/vec3.h"
#include "xo/geometry/quat.h"
#include "xo/geometry/quat_tmp.h"

using xo::vec3;
using xo::quat;
using xo::quatf;

namespace xo 
{
	struct dbl4 {
		dbl4( double x, double y, double z, double w ) { data[ 0 ] = x; data[ 1 ] = y; data[ 2 ] = z; data[ 3 ] = w; }
		double data[ 4 ];
	};
	template<> struct v4i< dbl4 >
	{
		using elem_t = double;
		static const elem_t& w( const dbl4& q ) { return q.data[ 3 ]; }
		static const elem_t& x( const dbl4& q ) { return q.data[ 0 ]; }
		static const elem_t& y( const dbl4& q ) { return q.data[ 1 ]; }
		static const elem_t& z( const dbl4& q ) { return q.data[ 2 ]; }
		static dbl4 make( elem_t w, elem_t x, elem_t y, elem_t z ) { return dbl4( x, y, z, w ); }
		const static bool value = true;
	};
}

void math_tmp_test()
{
	auto v1 = vec3( 1, 2, 3 );
	auto v2 = vec3( 4, 5, 6 );
	auto v3 = v2 - v1;
	auto q1 = xo::make_from_axes< quat >( v1, v2, v3 );
	auto v4 = q1 * v1;
	quatf qf = xo::make_from_axes< quatf >( v1, v2, v3 );
	auto v5 = qf * v1;
	auto qda = xo::make_from_quat< dbl4 >( qf );
}
