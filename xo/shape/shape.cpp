#include "shape.h"

#include "xo/utility/hash.h"
#include "xo/container/prop_node_tools.h"

namespace xo
{
	bool from_prop_node( const prop_node& pn, shape& s )
	{
		switch ( hash( pn.get<string>( "type" ) ) )
		{
		case "sphere"_hash:
			s = sphere{ pn.get<float>( "radius" ) };
			return true;
		case "cylinder"_hash:
			s = cylinder{ pn.get<float>( "radius" ), pn.get<float>( "height" ) };
			return true;
		case "capsule"_hash:
			s = capsule{ pn.get<float>( "radius" ), pn.get<float>( "height" ) };
			return true;
		case "box"_hash:
			s = box{ 0.5f * pn.get<vec3f>( "dim" ) };
			return true;
		case "plane"_hash:
			s = plane{ pn.get<vec3f>( "normal" ) };
			return true;
		default:
			return false;
		}
	}

	float volume( const shape& s )
	{
		return std::visit( []( auto&& arg ) { return volume( arg ); }, s );
	}

	void scale( shape& s, float f )
	{
		std::visit( [&]( auto&& arg ) { scale( arg, f ); }, s );
	}

	vec3f dim( const shape& s )
	{
		return std::visit( []( auto&& arg ) { return dim( arg ); }, s );
	}

	aabbf aabb( const shape& s, const transformf& t )
	{
		return std::visit( [&]( auto&& sarg ) { return aabb( sarg, t ); }, s );
	}

	vec3f inertia( const shape& s, float density )
	{
		return std::visit( [&]( auto&& arg ) { return inertia( arg, density ); }, s );
	}
}
