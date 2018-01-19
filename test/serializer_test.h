#pragma once
#include "xo/container/prop_node.h"
#include "xo/geometry/vec3_type.h"
#include "xo/serialization/serialize.h"
#include "xo/diagnose/test_framework.h"
#include "xo/system/log.h"

namespace xo
{
	prop_node make_prop_node()
	{
		xo::prop_node pn;
		pn.set( "empty", "" );
		//pn.set( "key with spaces", "value with spaces\nand \"special\" \001 characters" );
		pn.push_back( "test", 1.23f );
		pn.push_back( "duplicate", 1 );
		pn.push_back( "duplicate", 2 );

		std::vector< string > vec;
		for ( int i = 1; i <= 3; ++i ) vec.push_back( stringf( "name%d", i ) );
		pn.push_back( "vec_test", vec );

		std::vector< vec3f > vec2;
		for ( int i = 1; i <= 3; ++i ) vec2.push_back( vec3d( i, i * 1.1, i * 1.11 ) );
		pn.push_back( "vec2_test", vec2 );

		enum class enumclass { value1, value2, value3 };
		enum normalenum { value1, value2, value3 };
		enumclass e1 = enumclass::value3;
		normalenum e2 = value2;
		pn.push_back( "e1", e1 );
		pn.push_back( "e2", e2 );

		prop_node root;
		root.push_back( "root", pn );

		return root;
	}

	void serializer_test()
	{
		prop_node p1 = make_prop_node();

		std::stringstream str;
		str << prop_node_serializer< file_format::xml >( p1 );
		cout << str.str();

		prop_node p2;
		str >> p2;
		if ( !XO_TEST( p1 == p2 ) )
		{
			log::info( p1 );
			log::info( p2 );
		}
	}
}