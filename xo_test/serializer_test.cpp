#include "xo/container/prop_node.h"
#include "xo/geometry/vec3.h"
#include "xo/serialization/serialize.h"
#include "xo/system/test_case.h"
#include "xo/system/log.h"
#include "xo/utility/smart_enum.h"
#include "xo/serialization/prop_node_serializer_zml.h"
#include "xo/string/string_tools.h"
#include <sstream>

namespace xo
{
	xo_smart_enum_class( enumclass, value1, value2, value3 );
	xo_smart_enum( normalenum, value1, value2, value3 );

	prop_node example_prop_node()
	{
		xo::prop_node pn;
		pn.set( "empty", "" );
		pn.set( "key with spaces", "value with spaces\nand \"special\" \001 characters" );
		pn.add_key_value( "test", 1.23f );
		pn.add_key_value( "duplicate", 1 );
		pn.add_key_value( "duplicate", 2 );

		std::vector< string > vec;
		for ( int i = 1; i <= 3; ++i ) vec.push_back( stringf( "name%d", i ) );
		pn.add_key_value( "vec_test", vec );

		std::vector< vec3f > vec2;
		for ( int i = 1; i <= 3; ++i ) vec2.push_back( vec3f( vec3d( i, i * 1.1, i * 1.11 ) ) );
		pn.add_key_value( "vec2_test", vec2 );

		enumclass e1 = enumclass::value3;
		normalenum e2 = value2;
		pn.add_key_value( "e1", e1 );
		pn.add_key_value( "e2", e2 );

		prop_node root;
		root.add_child( "root", pn );

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
