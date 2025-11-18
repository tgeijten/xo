#include "xo/system/test_case.h"
#include "xo/utility/cached_var.h"

namespace xo
{
	XO_TEST_CASE( xo_cached_var_test )
	{
		cached_var<string, double> test( 0 );
		int update = 0;

		XO_CHECK( test( 0.0, [&]() { ++update; return "No update"; } ) == "" );
		XO_CHECK( test( 1.0, [&]() { ++update; return "Test1"; } ) == "Test1" );
		XO_CHECK( test( 1.0, [&]() { ++update; return "No update"; } ) == "Test1" );
		XO_CHECK( test( 2.0, [&]() { ++update; return "Test2"; } ) == "Test2" );
		XO_CHECK( update == 2 );
	}
}
