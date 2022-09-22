#include "xo/xo_types.h"
#include "xo/string/string_tools.h"
#include "xo/system/test_case.h"
#include "xo/filesystem/path.h"
#include "xo/string/dictionary.h"
#include "xo/utility/hash.h"
#include <sstream>
#include "xo/string/string_cast.h"
#include "xo/system/system_tools.h"
#include <filesystem>

namespace xo
{
	XO_TEST_CASE( xo_string_tools )
	{
		enum fruit { no_fruit, appel, peer, banaan, fruit_count };
		auto fruit_dict = xo::dictionary< fruit >( { { appel, "appel" }, { peer, "peer" }, { banaan, "banaan" } } );

		XO_CHECK( fruit_dict( "appel", no_fruit ) == appel );
		XO_CHECK( fruit_dict( "peer", no_fruit ) == peer );
		XO_CHECK( fruit_dict( "banaan", no_fruit ) == banaan );
		XO_CHECK( fruit_dict( "peerr", no_fruit ) == no_fruit );

		for ( int f = appel; f < fruit_count; ++f )
		{
			switch ( hash( fruit_dict( fruit( f ) ) ) )
			{
			case "appel"_hash: XO_CHECK( f == appel ); break;
			case "peer"_hash: XO_CHECK( f == peer ); break;
			case "banaan"_hash: XO_CHECK( f == banaan ); break;
			default: XO_CHECK_MESSAGE( 1 == 2, "hash error" );
			}
		}

		string s = "Test 0.123 24";
#if 0
		string a;
		double d;
		int i;
		scan_str( s, a, d, i );
		XO_CHECK( a == "Test" );
		XO_CHECK( d == 0.123 );
		XO_CHECK( i == 24 );
#endif // 0

		auto vs = split_str( s, " " );
		XO_CHECK( vs[ 0 ] == "Test" );
		XO_CHECK( vs[ 1 ] == "0.123" );
		XO_CHECK( vs[ 2 ] == "24" );

		auto vs2 = xo::split_str( "appel; peer,,, banaan", ";., " );
		XO_CHECK( vs2[ 0 ] == "appel" );
		XO_CHECK( vs2[ 1 ] == "peer" );
		XO_CHECK( vs2[ 2 ] == "banaan" );

		auto nasty_string = string( "String with \"quotes\" and spaces\nand\tspecial\001\002\003characters" );
		auto quoted_nasty = try_quoted( nasty_string );
		auto unquoted_nasty = try_unquoted( quoted_nasty );
		XO_CHECK( nasty_string == unquoted_nasty );

		path p( "X:/appel/taart.txt" );
		XO_CHECK( p.empty() == false );
		XO_CHECK( p.extension_no_dot() == "txt" );
		XO_CHECK( p.replace_extension( "new_ext" ).str() == "X:/appel/taart.new_ext" );
		XO_CHECK( p.filename() == "taart.new_ext");
		XO_CHECK( p.stem() == "taart");
		XO_CHECK( path( "foo/bar.txt" ).stem() == "bar" );
		XO_CHECK( path( "foo/.bar" ).stem() == "" );
		XO_CHECK( p.replace_filename( "new" ) == "X:/appel/new" );
		XO_CHECK( path( "X:/test" ) / "bla" == "X:/test/bla" );
		XO_CHECK( path( "X:/test/" ) / "bla" == "X:/test/bla" );
		XO_CHECK( path( "X:\\test\\bla" ).make_preferred() == "X:/test/bla" );
		XO_CHECK( path() / "SCONE" == "SCONE" );
		XO_CHECK( std::filesystem::path() / "SCONE" == "SCONE" );

		// some string testing
		string a = "apple";
		XO_CHECK( str_begins_with( a, "ap" ) );
		XO_CHECK( str_begins_with( a, "a" ) );
		XO_CHECK( str_begins_with( a, "apple" ) );
		XO_CHECK( !str_begins_with( a, "apples" ) );
		XO_CHECK( !str_begins_with( a, "p" ) );
		XO_CHECK( !str_begins_with( a, "e" ) );
		XO_CHECK( str_ends_with( a, "le" ) );
		XO_CHECK( str_ends_with( a, "e" ) );
		XO_CHECK( str_ends_with( a, "apple" ) );
		XO_CHECK( !str_ends_with( a, "bapple" ) );
		XO_CHECK( !str_ends_with( a, "a" ) );

		XO_CHECK( tidy_identifier( "::struct::_member_" ) == "member" );
		XO_CHECK( tidy_identifier( "a::b::c::d::e" ) == "e" );
		XO_CHECK( tidy_identifier( "a::b::apple_" ) == "apple" );
		XO_CHECK( tidy_identifier( "a::b::_banana" ) == "banana" );
		XO_CHECK( tidy_identifier( "a::b::__nope__" ) == "nope" );
		XO_CHECK( tidy_identifier( "data.member_" ) == "member" );
		XO_CHECK( tidy_identifier( "sub.data.member_" ) == "member" );

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

		std::stringstream str;
		str << fr;
	}
}
