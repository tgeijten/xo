#pragma once

#include "xo/system/assert.h"

namespace xo
{
	struct cylinder
	{
		float radius_;
		float height_;
	};

	inline float volume( const cylinder& s ) {
		return constantsf::pi() * squared( s.radius_ ) * s.height_;
	}

	inline aabbf aabb( const cylinder& s, const transformf& t ) {
		XO_NOT_IMPLEMENTED;
	}

	inline vec3f dim( const cylinder& s ) {
		return vec3f( s.radius_, s.radius_, s.height_ );
	}

	inline vec3f inertia( const cylinder& s, float density ) {
		vec3f r;
		auto m = density * volume( s );
		r.x = r.y = m / 12.0f * ( 3 * squared( s.radius_ ) + squared( s.height_ ) );
		r.z = 0.5f * m * squared( s.radius_ ); // cylinder along z axis
		return r;
	}

	inline void scale( cylinder& s, float f ) {
		s.radius_ *= f;
		s.height_ *= f;
	}
}
