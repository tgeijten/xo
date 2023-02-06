#pragma once

#include "xo/system/assert.h"
#include <cmath>

namespace xo
{
	/// cone shape, y-up
	struct cone
	{
		cone() : radius_(), height_() {}
		explicit cone( float radius, float height ) : radius_( radius ), height_( height ) {}

		float radius_;
		float height_;
	};

	inline float volume( const cone& s ) {
		return constantsf::pi() * squared( s.radius_ ) * s.height_ / 3.0f;
	}

	inline vec3f dim( const cone& s ) {
		return vec3f( 2.0f * s.radius_, s.height_, 2.0f * s.radius_ );
	}

	inline aabbf aabb( const cone& s, const transformf& t ) {
		XO_NOT_IMPLEMENTED;
	}

	inline vec3f inertia( const cone& s, float density ) {
		XO_NOT_IMPLEMENTED;
	}

	inline void scale( cone& s, float f ) {
		s.radius_ *= f;
		s.height_ *= f;
	}

	inline void scale( cone& s, const vec3f& sv ) {
		s.radius_ *= std::sqrt( sv.x * sv.z );
		s.height_ *= sv.y;
	}
}
