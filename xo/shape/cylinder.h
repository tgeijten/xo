#pragma once

#include "xo/system/assert.h"
#include <cmath>

namespace xo
{
	/// cylinder shape, y-up
	struct cylinder
	{
		cylinder() : radius_(), height_() {}
		explicit cylinder( float radius, float height ) : radius_( radius ), height_( height ) {}

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
		return vec3f( 2.0f * s.radius_, s.height_, 2.0f * s.radius_ );
	}

	inline vec3f inertia( const cylinder& s, float density ) {
		vec3f r;
		auto m = density * volume( s );
		r.x = r.z = m / 12.0f * ( 3 * squared( s.radius_ ) + squared( s.height_ ) );
		r.y = 0.5f * m * squared( s.radius_ ); // cylinder along y axis
		return r;
	}

	inline void scale( cylinder& s, float f ) {
		s.radius_ *= f;
		s.height_ *= f;
	}

	inline void scale( cylinder& s, const vec3f& sv ) {
		s.radius_ *= std::sqrt( sv.x * sv.z );
		s.height_ *= sv.y;
	}
}
