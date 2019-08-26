#pragma once

#include "vec3_type.h"

namespace xo
{
	template< typename T >
	struct ray_
	{
		ray_() : pos_(), dir_() {}
		ray_( const vec3_<T>& pos, const vec3_<T>& dir ) : pos_( pos ), dir_( dir ) {}

		vec3_<T> pos_;
		vec3_<T> dir_;
	};

	using rayf = ray_< float >;
	using rayd = ray_< double >;
}
