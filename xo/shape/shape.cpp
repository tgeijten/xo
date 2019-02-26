#include "shape.h"

#include "xo/container/prop_node_tools.h"
#include "xo/string/dictionary.h"
#include "xo/utility/hash.h"
#include <variant>

namespace xo
{

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
