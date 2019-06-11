#pragma once
#include "xo/container/prop_node.h"
#include "xo/geometry/vec3_type.h"
#include "xo/serialization/serialize.h"
#include "xo/system/test_framework.h"
#include "xo/system/log.h"
#include "xo/serialization/prop_node_serializer_zml.h"
#include <sstream>

namespace xo
{
	prop_node example_prop_node()
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
		pn.push_back( "e1", (int)e1 );
		pn.push_back( "e2", (int)e2 );

		prop_node root;
		root.push_back( "root", pn );

		return root;
	}

	XO_TEST_CASE( xo_serializer_test )
	{
		prop_node p1 = example_prop_node();
		prop_node p2;

		std::stringstream str;
		prop_node_serializer_zml( p1 ).write_stream( str );
		prop_node_serializer_zml( p2 ).read_stream( str );

		XO_CHECK( p1 == p2 );
		if ( p1 != p2 )
		{
			log::info( p1 );
			log::info( str.str() );
			log::info( p2 );
		}
	}
}