#pragma once

#include <iostream>
#include "xo/container/prop_node_tools.h"
#include "xo/string/string_cast_enum.h"
#include "xo/geometry/vec3_type.h"
#include "xo/container/container_tools.h"
#include "xo/serialization/serialize.h"
#include "xo/system/test_framework.h"
#include "xo/filesystem/filesystem.h"
#include "xo/system/log.h"

namespace xo
{
	struct custom_struct
	{
		string name;
		double value;
	};

	inline prop_node to_prop_node( const custom_struct& value )
	{
		prop_node pn;
		pn.set( "name", value.name );
		pn.set( "value", value.value );
		return pn;
	}

	inline bool from_prop_node( const prop_node& pn, custom_struct& cs )
	{
		cs.name = pn.get<string>( "name" );
		cs.value = pn.get<double>( "value" );
		return true;
	}

	XO_TEST_CASE( xo_prop_node_test )
	{
		const prop_node literal_pn( "a=appel b=123" );
		XO_CHECK( literal_pn.get< string >( "a" ) == "appel" );
		XO_CHECK( literal_pn.get< int >( "b" ) == 123 );

		enum class enumclass { value1, value2, value3 };
		enum normalenum { value1, value2, value3 };

		enumclass e1 = enumclass::value3;
		normalenum e2 = value2;

		xo::prop_node pn;
		pn.set( "empty", "" );
		pn.set( "key with spaces", "value with spaces\nand \"special\" \001 characters" );
		pn.push_back( "test", 1.23f );
		XO_CHECK( pn.get< float >( "test" ) == 1.23f );

		pn.push_back( "duplicate", 1 );
		pn.push_back( "duplicate", 2 );

		std::vector< custom_struct > vec;
		for ( int i = 1; i <= 3; ++i ) vec.push_back( custom_struct{ stringf( "name%d", i ), i * 1.5 } );
		pn.push_back( "vec_test", vec );

		std::vector< vec3f > vec2;
		for ( int i = 1; i <= 3; ++i ) vec2.push_back( vec3d( i, i * 1.1, i * 1.11 ) );
		pn.push_back( "vec2_test", vec2 );

		pn.push_back( "e1", e1 );
		pn.push_back( "e2", e2 );

		auto vec1b = pn.get< std::vector< custom_struct > >( "vec_test" );
		auto vec2b = pn.get< std::vector< vec3f > >( "vec2_test" );

		XO_CHECK( e1 == pn.get< enumclass >( "e1" ) );
		XO_CHECK( e2 == pn.get< normalenum >( "e2" ) );

		prop_node p2;
		p2.set( "test", pn );
		XO_CHECK( p2.get_child( "test" ) == pn );

		pn.push_back( "duplicate", 3 );

		// test delimiters
		pn.set_query( "this.is.a.subfolder", 1.5 );
		XO_CHECK( pn[ "this" ][ "is" ][ "a" ][ "subfolder" ].get< double >() == 1.5 );
		//XO_CHECK( pn.get_delimited< double >( "this.is.a.subfolder" ) == 1.5 );

		pn.push_back( "duplicate", 4 );

		int count_s = 0;
		for ( auto& selection : pn.select( "duplicate" ) )
			XO_CHECK( selection.second.get< int >() == ++count_s );
		XO_CHECK( count_s == 4 );

		int count_v = 0;
		auto view = pn.select( "duplicate" );
		for ( auto it = view.begin(); it != view.end(); ++it )
			XO_CHECK( it->second.get< int >() == ++count_v );
		XO_CHECK( count_v == 4 );

		int count_sp = 0;
		for ( auto& selection : pn.select_pattern( "*test" ) )
			++count_sp;
		XO_CHECK( count_sp == 3 );

		auto filename = temp_directory_path() / "prop_node_test.zml";
		save_file( pn, filename );
		auto pn_loaded = load_file( filename );

		XO_CHECK( pn == pn_loaded );
		if ( pn != pn_loaded )
		{
			log::info( pn );
			log::info( pn_loaded );
		}
	}
}
