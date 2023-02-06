#include "shape.h"

#include "xo/utility/hash.h"
#include "xo/container/prop_node_tools.h"
#include "xo/utility/overload.h"

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
		s.normal_ = pn.get<vec3f>( "normal", vec3f::unit_x() );
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

	prop_node to_prop_node( const sphere& s )
	{
		prop_node pn;
		pn["type"] = "sphere";
		pn["radius"] = s.radius_;
		return pn;
	}

	prop_node to_prop_node( const box& s )
	{
		prop_node pn;
		pn["type"] = "box";
		pn["dim"] = 2 * s.half_dim_;
		return pn;
	}

	prop_node to_prop_node( const cylinder& s )
	{
		prop_node pn;
		pn["type"] = "cylinder";
		pn["radius"] = s.radius_;
		pn["height"] = s.height_;
		return pn;
	}
	prop_node to_prop_node( const capsule& s )
	{
		prop_node pn;
		pn["type"] = "capsule";
		pn["radius"] = s.radius_;
		pn["height"] = s.height_;
		return pn;
	}

	prop_node to_prop_node( const cone& s )
	{
		prop_node pn;
		pn["type"] = "cone";
		pn["radius"] = s.radius_;
		pn["height"] = s.height_;
		return pn;
	}

	prop_node to_prop_node( const plane& s )
	{
		prop_node pn;
		pn["type"] = "plane";
		pn["normal"] = s.normal_;
		return pn;
	}

	string to_str( const sphere& s )
	{
		return "sphere( radius=" + xo::to_str( s.radius_ ) + " )";
	}

	string to_str( const box& s )
	{
		return "box( half_dim=" + xo::to_str( s.half_dim_ ) + " )";
	}

	string to_str( const cylinder& s )
	{
		return "cylinder( radius=" + xo::to_str( s.radius_ ) + ", height=" + xo::to_str( s.height_ ) + " )";
	}

	string to_str( const capsule& s )
	{
		return "capsule( radius=" + xo::to_str( s.radius_ ) + ", height=" + xo::to_str( s.height_ ) + " )";
	}

	string to_str( const cone& s )
	{
		return "cone( radius=" + xo::to_str( s.radius_ ) + ", height=" + xo::to_str( s.height_ ) + " )";
	}

	string to_str( const plane& s )
	{
		return "plane( normal=" + xo::to_str( s.normal_ ) + " )";
	}
}
