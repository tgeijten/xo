#include "xo/system/test_case.h"
#include "xo/utility/index.h"

namespace xo
{
	XO_TEST_CASE( xo_index_test )
	{
		std::vector<double> v{ 1.0, 2.0, 3.0, 4.0 };
		auto idx = xo::index_class<double>( 1 );

		XO_CHECK( v[ idx.value_ ] == 2.0 );
		XO_CHECK( v[ ( idx + 1 ).value_ ] == 3.0 );
		auto i3 = idx + 2;
		XO_CHECK( v[ i3.value_ ] == 4.0 );
	}
}
