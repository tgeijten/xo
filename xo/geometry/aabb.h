#pragma once

#include "vec3_type.h"
#include "xo/numerical/constants.h"

namespace xo
{
	template< typename T >
	struct aabb_
	{
		aabb_() : lower_bounds( constants<T>::max() ), upper_bounds( constants<T>::lowest() ) {}
		aabb_( const vec3_<T>& p ) : lower_bounds( p ), upper_bounds( p ) {}
		aabb_( const vec3_<T>& lower, const vec3_<T>& upper ) : lower_bounds( lower ), upper_bounds( upper ) {}

		aabb_<T>& operator+=( const vec3_<T>& v ) {
			set_if_smaller( lower_bounds.x, v.x );
			set_if_smaller( lower_bounds.y, v.y );
			set_if_smaller( lower_bounds.z, v.z );
			set_if_bigger( upper_bounds.x, v.x );
			set_if_bigger( upper_bounds.y, v.y );
			set_if_bigger( upper_bounds.z, v.z );
			return *this;
		}

		aabb_<T>& operator+=( const aabb_<T>& bb ) {
			set_if_smaller( lower_bounds.x, bb.lower_bounds.x );
			set_if_smaller( lower_bounds.y, bb.lower_bounds.y );
			set_if_smaller( lower_bounds.z, bb.lower_bounds.z );
			set_if_bigger( upper_bounds.x, bb.upper_bounds.x );
			set_if_bigger( upper_bounds.y, bb.upper_bounds.y );
			set_if_bigger( upper_bounds.z, bb.upper_bounds.z );
			return *this;
		}

		vec3_<T> lower_bounds;
		vec3_<T> upper_bounds;
	};

	using aabbf = aabb_< float >;
	using aabbd = aabb_< double >;
}
