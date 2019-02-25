#include "shape.h"

#include "xo/container/prop_node_tools.h"
#include "xo/string/dictionary.h"
#include "xo/utility/hash.h"
#include <variant>

namespace xo
{
	std::array< vec3f, 8 > box::corners() const
	{
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

	shape make_shape( const prop_node& pn )
	{
		switch ( hash( pn[ "type" ].get_value() ) )
		{
		case "sphere"_hash: return sphere{ pn.get<float>( "radius" ) };
		case "cylinder"_hash: return cylinder{ pn.get<float>( "radius" ), pn.get<float>( "height" ) };
		case "capsule"_hash: return capsule{ pn.get<float>( "radius" ), pn.get<float>( "height" ) };
		case "box"_hash: return box{ 0.5f * pn.get<vec3f>( "dim" ) };
		case "plane"_hash: return plane{ pn.get<vec3f>( "normal" ) };
		default: xo_error( "Unknown shape type: " + pn.get<string>( "type" ) );
		}
	}

	float volume( const shape& s )
	{
		return std::visit( []( auto&& arg ) { return arg.volume(); }, s );
	}

	void scale( shape& s, float f )
	{
		std::visit( [&]( auto&& arg ) { scale( arg, f ); }, s );
	}

	vec3f dim( const shape& s )
	{
		return std::visit( []( auto&& arg ) { return arg.dim(); }, s );
	}

	aabbf aabb( const sphere& s, const transformf& t )
	{
		return aabbf( t.p - vec3f( s.radius_ ), t.p + vec3f( s.radius_ ) );
	}

	aabbf aabb( const cylinder& s, const transformf& t )
	{
		XO_NOT_IMPLEMENTED;
	}

	aabbf aabb( const capsule& s, const transformf& t )
	{
		XO_NOT_IMPLEMENTED;
	}

	aabbf aabb( const box& s, const transformf& t )
	{
		aabbf bb;
		auto clist = s.corners();
		for ( index_t i = 0; i < 8; ++i )
			bb += t * clist[ i ];
		return bb;
	}

	aabbf aabb( const plane& s, const transformf& t )
	{
		xo_error( "Cannot compute aabb of plane" );
	}

	aabbf aabb( const shape& s, const transformf& t )
	{
		return std::visit( [&]( auto&& sarg ) { return aabb( sarg, t ); }, s );
	}

	aabbf aabb( const cone& s, const transformf& t )
	{
		XO_NOT_IMPLEMENTED;
	}

	vec3f inertia( const sphere& s, float density )
	{
		return  vec3f( ( 2.0f / 5.0f ) * volume( s ) * density * squared( s.radius_ ) );
	}

	vec3f inertia( const cylinder& s, float density )
	{
		vec3f r;
		auto m = density * volume( s );
		r.x = m / 12.0f * ( 3 * squared( s.radius_ ) + squared( s.height_ ) );
		r.y = r.x;
		r.z = 0.5f * m * squared( s.radius_ ); // cylinder along z axis
		return r;
	}

	vec3f inertia( const capsule& s, float density )
	{
		XO_NOT_IMPLEMENTED;
	}

	vec3f inertia( const box& s, float density )
	{
		vec3f r;
		auto m = density * volume( s );
		r.x = m / 12.0f * ( squared( s.half_dim_.y ) * 4.0f + squared( s.half_dim_.z ) * 4.0f );
		r.y = m / 12.0f * ( squared( s.half_dim_.x ) * 4.0f + squared( s.half_dim_.z ) * 4.0f );
		r.z = m / 12.0f * ( squared( s.half_dim_.x ) * 4.0f + squared( s.half_dim_.y ) * 4.0f );;
		return r;
	}

	vec3f inertia( const plane& s, float density )
	{
		xo_error( "Cannot compute inertia of plane" );
	}

	vec3f inertia( const shape& s, float density )
	{
		return std::visit( [&]( auto&& arg ) { return inertia( arg, density ); }, s );
	}

	vec3f inertia( const cone& s, float density )
	{
		XO_NOT_IMPLEMENTED;
	}
}
