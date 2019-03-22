#pragma once
#include "xo/xo_types.h"
#include "xo/string/string_tools.h"
#include "xo/diagnose/test_framework.h"
#include "xo/filesystem/path.h"
#include "xo/string/dictionary.h"
#include "xo/utility/hash.h"

namespace xo
{
	void string_test()
	{
		enum fruit { no_fruit, appel, peer, banaan, fruit_count };
		auto fruit_dict = xo::dictionary< fruit >( { { appel, "appel" }, { peer, "peer" }, { banaan, "banaan" } } );

		XO_TEST( fruit_dict( "appel", no_fruit ) == appel );
		XO_TEST( fruit_dict( "peer", no_fruit ) == peer );
		XO_TEST( fruit_dict( "banaan", no_fruit ) == banaan );
		XO_TEST( fruit_dict( "peerr", no_fruit ) == no_fruit );

		for ( int f = appel; f < fruit_count; ++f )
		{
			switch ( hash( fruit_dict( fruit( f ) ) ) )
			{
			case "appel"_hash: XO_TEST( f == appel ); break;
			case "peer"_hash: XO_TEST( f == peer ); break;
			case "banaan"_hash: XO_TEST( f == banaan ); break;
			default: XO_TEST_MSG( 1 == 2, "hash error" );
			}
		}

		string s = "Test 0.123 24";
#if 0
		string a;
		double d;
		int i;
		scan_str( s, a, d, i );
		XO_TEST( a == "Test" );
		XO_TEST( d == 0.123 );
		XO_TEST( i == 24 );
#endif // 0

		auto vs = split_str( s, " " );
		XO_TEST( vs[ 0 ] == "Test" );
		XO_TEST( vs[ 1 ] == "0.123" );
		XO_TEST( vs[ 2 ] == "24" );

		auto vs2 = xo::split_str( "appel; peer,,, banaan", ";., " );
		XO_TEST( vs2[ 0 ] == "appel" );
		XO_TEST( vs2[ 1 ] == "peer" );
		XO_TEST( vs2[ 2 ] == "banaan" );

		auto nasty_string = string( "String with \"quotes\" and spaces\nand\tspecial\001\002\003characters" );
		auto quoted_nasty = try_quoted( nasty_string );
		auto unquoted_nasty = try_unquoted( quoted_nasty );
		XO_TEST( nasty_string == unquoted_nasty );

		path p( "X:/appel/taart.txt" );
		XO_TEST( p.empty() == false );
		XO_TEST( p.extension() == "txt" );
		XO_TEST( p.replace_extension( "new_ext" ).string() == "X:/appel/taart.new_ext" );
		XO_TEST( p.filename() == "taart.new_ext");
		XO_TEST( p.stem() == "taart");
		XO_TEST( path( "foo/bar.txt" ).stem() == "bar" );
		XO_TEST( path( "foo/.bar" ).stem() == "" );
		XO_TEST( p.replace_filename( "new" ) == "X:/appel/new" );
		XO_TEST( path( "X:/test" ) / "bla" == "X:/test/bla" );
		XO_TEST( path( "X:/test/" ) / "bla" == "X:/test/bla" );
		XO_TEST( path( "X:\\test\\bla" ).make_preferred() == "X:/test/bla" );

		index_t idx = 123;
		int i = -10;
		double d = 1.0 / 3.0;
		float f = 1.0f / 3.0f;
		fruit fr = peer;

		auto sidx = to_str( idx );
		auto si = to_str( i );
		auto sd = to_str( d );
		auto sf = to_str( f );
		auto sfr = to_str( fr );
	}
}
