#pragma once

namespace xo
{
	struct capsule
	{
		float radius_;
		float height_;
	};

	inline float volume( const capsule& s ) {
		return pi_f * squared( s.radius_ ) * ( ( 4.0f / 3.0f ) * ( s.radius_ + s.height_ ) );
	}

	inline vec3f dim( const capsule& s ) {
		return vec3f( s.radius_, s.radius_, s.height_ );
	}
	
	inline aabbf aabb( const capsule& s, const transformf& t ) {
		XO_NOT_IMPLEMENTED;
	}
	
	inline vec3f inertia( const capsule& s, float density ) {
		XO_NOT_IMPLEMENTED;
	}
	
	inline void scale( capsule& s, float f ) {
		s.radius_ *= f; s.height_ *= f;
	}
}
