#pragma once

#include "xo/container/prop_node_tools.h"
#include <iostream>
#include "xo/geometry/vec3_type.h"
#include "xo/container/container_tools.h"
#include "xo/serialization/serialize.h"

namespace xo
{
	struct custom_struct
	{
		custom_struct( const string& n, double v ) : name( n ), value ( v ) {}
		string name;
		double value;
	};


	template<> struct prop_node_cast< custom_struct > {
		static custom_struct from( const prop_node& pn )
		{
			return custom_struct( pn.get<string>( "name" ), pn.get<double>( "value" ) );
		}
		static prop_node to( const custom_struct& value )
		{
			prop_node pn;
			pn.set( "name", value.name );
			pn.set( "value", value.value );
			return pn;
		}
	};

	void prop_node_test()
	{
		const prop_node literal_pn( "a=appel b=123" );
		XO_TEST( literal_pn.get< string >( "a" ) == "appel" );
		XO_TEST( literal_pn.get< int >( "b" ) == 123 );

		enum class enumclass { value1, value2, value3 };
		enum normalenum { value1, value2, value3 };

		enumclass e1 = enumclass::value3;
		normalenum e2 = value2;

		xo::prop_node pn;
		pn.set( "empty", "" );
		pn.set( "key with spaces", "value with spaces\nand \"special\" \001 characters" );
		pn.push_back( "test", 1.23f );
		XO_TEST( pn.get< float >( "test" ) == 1.23f );

		pn.push_back( "duplicate", 1 );
		pn.push_back( "duplicate", 2 );

		std::vector< custom_struct > vec;
		for ( int i = 1; i <= 3; ++i ) vec.push_back( custom_struct( stringf( "name%d", i ), i * 1.5 ) );
		pn.push_back( "vec_test", vec );

		std::vector< vec3f > vec2;
		for ( int i = 1; i <= 3; ++i ) vec2.push_back( vec3d( i, i * 1.1, i * 1.11 ) );
		pn.push_back( "vec2_test", vec2 );

		pn.push_back( "e1", e1 );
		pn.push_back( "e2", e2 );

		auto vec1b = pn.get< std::vector< custom_struct > >( "vec_test" );
		auto vec2b = pn.get< std::vector< vec3f > >( "vec2_test" );

		XO_TEST( e1 == pn.get< enumclass >( "e1" ) );
		XO_TEST( e2 == pn.get< normalenum >( "e2" ) );

		prop_node p2;
		p2.set( "test", pn );
		XO_TEST( p2.get_child( "test" ) == pn );

		pn.push_back( "duplicate", 3 );

		// test delimiters
		pn.set_query( "this.is.a.subfolder", 1.5 );
		XO_TEST( pn[ "this" ][ "is" ][ "a" ][ "subfolder" ].get< double >() == 1.5 );
		//XO_TEST( pn.get_delimited< double >( "this.is.a.subfolder" ) == 1.5 );

		pn.push_back( "duplicate", 4 );

		int count_s = 0;
		for ( auto& selection : pn.select( "duplicate" ) )
			XO_TEST( selection.second.get< int >() == ++count_s );
		XO_TEST( count_s == 4 );

		int count_v = 0;
		auto view = pn.select( "duplicate" );
		for ( auto it = view.begin(); it != view.end(); ++it )
			XO_TEST( it->second.get< int >() == ++count_v );
		XO_TEST( count_v == 4 );

		int count_sp = 0;
		for ( auto& selection : pn.select_pattern( "*test" ) )
			++count_sp;
		XO_TEST( count_sp == 3 );

		save_file( pn, "prop_node_test.zml" );
		auto pn_loaded = load_file( "prop_node_test.zml" );

		XO_TEST( pn == pn_loaded );
		if ( pn != pn_loaded )
		{
			log::info( pn );
			log::info( pn_loaded );
		}
	}
}
