#pragma once

#include "vec3_type.h"
#include "xo/numerical/constants.h"

namespace xo
{
	template< typename T >
	struct bounding_box_
	{
		bounding_box_() : lower_bounds( max<T>() ), upper_bounds( lowest<T>() ) {}
		bounding_box_( const vec3_<T>& p ) : lower_bounds( p ), upper_bounds( p ) {}
		bounding_box_( const vec3_<T>& lower, const vec3_<T>& upper ) : lower_bounds( lower ), upper_bounds( upper ) {}

		bounding_box_<T>& operator+=( const vec3_<T>& v ) {
			set_if_smaller( lower_bounds.x, v.x );
			set_if_smaller( lower_bounds.y, v.y );
			set_if_smaller( lower_bounds.z, v.z );
			set_if_bigger( upper_bounds.x, v.x );
			set_if_bigger( upper_bounds.y, v.y );
			set_if_bigger( upper_bounds.z, v.z );
			return *this;
		}

		bounding_box_<T>& operator+=( const bounding_box_<T>& bb ) {
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

	using bounding_boxf = bounding_box_< float >;
	using bounding_boxd = bounding_box_< double >;
}
