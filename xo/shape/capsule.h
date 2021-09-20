#pragma once

#include "xo/system/assert.h"
#include <cmath>

namespace xo
{
	/// capsule shape, y-up, height is excluding cylinder caps
	struct capsule
	{
		capsule() : radius_(), height_() {}
		explicit capsule( float radius, float height ) : radius_( radius ), height_( height ) {}

		float radius_;
		float height_;
	};

	inline float volume( const capsule& s ) {
		return constantsf::pi() * squared( s.radius_ ) * ( ( 4.0f / 3.0f ) * s.radius_ + s.height_ );
	}

	inline vec3f dim( const capsule& s ) {
		return vec3f( 2.0f * s.radius_, s.height_, 2.0f * s.radius_ );
	}
	
	inline aabbf aabb( const capsule& s, const transformf& t ) {
		XO_NOT_IMPLEMENTED;
	}
	
	inline vec3f inertia( const capsule& s, float density ) {
		const auto h = s.height_;
		const auto r = s.radius_;
		const auto r2 = r * r;
		const auto r3 = r2 * r;
		const auto pi = num<float>::pi;

		float m1 = pi * r2 * h * density;
		float m2 = ( 4.0f / 3.0f ) * pi * r3 * density;
		float Ia = m1 * ( 0.25f * r2 + ( 1.0f / 12.0f ) * m1 * h * h ) + m2 * ( 0.4f * r2 + 0.375f * r * h + 0.25f * h * h );
		float Ib = ( m1 * 0.5f + m2 * 0.4f ) * r2;

		return vec3f{ Ia, Ib, Ia };
	}
	
	inline void scale( capsule& s, float f ) {
		s.radius_ *= f;
		s.height_ *= f;
	}

	inline void scale( capsule& s, const vec3f& sv ) {
		s.radius_ *= std::sqrt( sv.x * sv.z );
		s.height_ *= sv.y;
	}

	inline pair<vec3f, vec3f> center_positions( const capsule& s, const transformf& t ) {
		return { t.p, t.p + s.height_ * local_y_axis( t.q ) };
	}
}
