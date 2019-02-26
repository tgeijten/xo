#pragma once

#include "xo/geometry/vec3.h"
#include <array>

namespace xo
{
	struct box
	{
		vec3f half_dim_;

		std::array< vec3f, 8 > corners() const;
	};

	inline float volume( const box& s ) {
		return 8.0f * s.half_dim_.x * s.half_dim_.y * s.half_dim_.z;
	}

	inline aabbf aabb( const box& s, const transformf& t ) {
		aabbf bb;
		auto clist = s.corners();
		for ( index_t i = 0; i < 8; ++i )
			bb += t * clist[ i ];
		return bb;
	}

	inline vec3f dim( const box& s ) {
		return 2.0f * s.half_dim_;
	}

	inline vec3f inertia( const box& s, float density ) {
		vec3f r;
		auto m = density * volume( s );
		r.x = m / 3.0f * ( squared( s.half_dim_.y ) + squared( s.half_dim_.z ) );
		r.y = m / 3.0f * ( squared( s.half_dim_.x ) + squared( s.half_dim_.z ) );
		r.z = m / 3.0f * ( squared( s.half_dim_.x ) + squared( s.half_dim_.y ) );;
		return r;
	}

	inline void scale( box& s, float f ) {
		s.half_dim_ *= f;
	}

	inline std::array< vec3f, 8 > box::corners() const {
		return std::array< vec3f, 8 >{
			{
				{ -half_dim_.x, -half_dim_.y, -half_dim_.z },
				{ half_dim_.x, -half_dim_.y, -half_dim_.z },
				{ -half_dim_.x, half_dim_.y, -half_dim_.z },
				{ half_dim_.x, half_dim_.y, -half_dim_.z },
				{ -half_dim_.x, -half_dim_.y, half_dim_.z },
				{ half_dim_.x, -half_dim_.y, half_dim_.z },
				{ -half_dim_.x, half_dim_.y, half_dim_.z },
				{ half_dim_.x, half_dim_.y, half_dim_.z }
			}
		};
	}
}
