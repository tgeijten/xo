#include "shape.h"

#include "xo/utility/hash.h"
#include "xo/container/prop_node_tools.h"

namespace xo
{
	bool from_prop_node( const prop_node& pn, sphere& s )
	{
		s.radius_ = pn.get<float>( "radius" );
		return true;
	}

	bool from_prop_node( const prop_node& pn, box& s )
	{
		s.half_dim_ = 0.5f * pn.get<vec3f>( "dim" );
		return true;
	}

	bool from_prop_node( const prop_node& pn, cylinder& s )
	{
		s.radius_ = pn.get<float>( "radius" );
		s.height_ = pn.get<float>( "height" );
		return true;
	}
	bool from_prop_node( const prop_node& pn, capsule& s )
	{
		s.radius_ = pn.get<float>( "radius" );
		s.height_ = pn.get<float>( "height" );
		return true;
	}

	bool from_prop_node( const prop_node& pn, cone& s )
	{
		s.radius_ = pn.get<float>( "radius" );
		s.height_ = pn.get<float>( "height" );
		return true;
	}

	bool from_prop_node( const prop_node& pn, plane& s )
	{
		s.normal_= pn.get<vec3f>( "normal", vec3f::unit_x() );
		return true;
	}

	bool from_prop_node( const prop_node& pn, shape& s )
	{
		switch ( hash( pn.get<string>( "type" ) ) )
		{
		case "sphere"_hash:
			return from_prop_node( pn, std::get<sphere>( s = sphere() ) );
		case "box"_hash:
			return from_prop_node( pn, std::get<box>( s = box() ) );
		case "cylinder"_hash:
			return from_prop_node( pn, std::get<cylinder>( s = cylinder() ) );
		case "capsule"_hash:
			return from_prop_node( pn, std::get<capsule>( s = capsule() ) );
		case "cone"_hash:
			return from_prop_node( pn, std::get<cone>( s = cone() ) );
		case "plane"_hash:
			return from_prop_node( pn, std::get<plane>( s = plane() ) );
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
