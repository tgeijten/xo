#pragma once

#include "xo/numerical/constants.h"
#include "xo/numerical/math.h"
#include "xo/geometry/vec3_type.h"
#include "xo/geometry/transform.h"
#include "xo/shape/aabb.h"

namespace xo
{
	struct sphere
	{
		float radius_;
	};

	inline float volume( const sphere& s ) {
		return ( 4.0f / 3.0f ) * constantsf::pi() * cubed( s.radius_ );
	}

	inline aabbf aabb( const sphere& s, const transformf& t ) {
		return aabbf( t.p - vec3f( s.radius_ ), t.p + vec3f( s.radius_ ) );
	}

	inline vec3f dim( const sphere& s ) {
		return vec3f( 2.0f * s.radius_ );
	}

	inline vec3f inertia( const sphere& s, float density )	{
		return  vec3f( ( 2.0f / 5.0f ) * volume( s ) * density * squared( s.radius_ ) );
	}

	inline void scale( sphere& s, float f ) {
		s.radius_ *= f;
	}
}
