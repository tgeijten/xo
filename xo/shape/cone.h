#pragma once

#include "xo/system/assert.h"

namespace xo
{
	struct cone
	{
		float radius_;
		float height_;
	};

	inline float volume( const cone& s ) {
		return constantsf::pi() * squared( s.radius_ ) * s.height_ / 3.0f;
	}

	inline vec3f dim( const cone& s ) {
		return vec3f( s.radius_, s.radius_, s.height_ );
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
}
