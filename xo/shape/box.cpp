#include "box.h"

#include "aabb.h"
#include "xo/geometry/transform.h"

namespace xo
{
	
	float volume( const box& s )
	{
		return 8.0f * s.half_dim_.x * s.half_dim_.y * s.half_dim_.z;
	}

	aabb_<float> aabb( const box& s, const transform_<float>& t )
	{
		aabbf bb;
		auto clist = corners( s );
		for ( index_t i = 0; i < 8; ++i )
			bb += t * clist[ i ];
		return bb;
	}

	vec3f dim( const box& s )
	{
		return 2.0f * s.half_dim_;
	}

	vec3f inertia( const box& s, float density )
	{
		vec3f r;
		auto m = density * volume( s );
		r.x = m / 3.0f * ( squared( s.half_dim_.y ) + squared( s.half_dim_.z ) );
		r.y = m / 3.0f * ( squared( s.half_dim_.x ) + squared( s.half_dim_.z ) );
		r.z = m / 3.0f * ( squared( s.half_dim_.x ) + squared( s.half_dim_.y ) );;
		return r;
	}

	void scale( box& s, float f )
	{
		s.half_dim_ *= f;
	}

	void scale( box& s, const vec3f& sv )
	{
		xo::scale( s.half_dim_, sv );
	}

	array< vec3f, 8 > corners( const box& b )
	{
		return array< vec3f, 8 >{
			{
				{ -b.half_dim_.x, -b.half_dim_.y, -b.half_dim_.z },
				{ b.half_dim_.x, -b.half_dim_.y, -b.half_dim_.z },
				{ -b.half_dim_.x, b.half_dim_.y, -b.half_dim_.z },
				{ b.half_dim_.x, b.half_dim_.y, -b.half_dim_.z },
				{ -b.half_dim_.x, -b.half_dim_.y, b.half_dim_.z },
				{ b.half_dim_.x, -b.half_dim_.y, b.half_dim_.z },
				{ -b.half_dim_.x, b.half_dim_.y, b.half_dim_.z },
				{ b.half_dim_.x, b.half_dim_.y, b.half_dim_.z }
			}
		};
	}

}
