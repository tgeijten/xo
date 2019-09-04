#include "xo/system/test_case.h"
#include "xo/utility/smart_enum.h"

namespace xo
{
	xo_smart_enum( fruit, apple, pear, banana );

	XO_TEST_CASE( xo_enum_test )
	{
		fruit f = apple;
		XO_CHECK( to_str( f ) == "apple" );
		XO_CHECK( from_str( "banana", f ) );
		XO_CHECK( f == banana );
		XO_CHECK( from_str( "apple", f ) );
		XO_CHECK( f == apple );
		XO_CHECK( to_str( banana ) == "banana" );
		XO_CHECK( !from_str( "nonona", f ) );
		XO_CHECK( !from_str( "", f ) );
		XO_CHECK( !from_str( "b", f ) );
		XO_CHECK( !from_str( "banan", f ) );
		XO_CHECK( !from_str( "banana ", f ) );
		XO_CHECK( !from_str( "bananana", f ) );
		XO_CHECK( f == apple );
	}
}
