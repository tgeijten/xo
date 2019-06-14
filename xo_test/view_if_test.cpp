#pragma once

#include "xo/container/view_if.h"
#include "xo/system/test_framework.h"
#include <vector>

namespace xo
{
	XO_TEST_CASE( xo_view_if_test )
	{
		std::vector<int> v{ 3, 1, 4, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9 };
		int count = 0;
		for ( auto& i : make_view_if( v, [&]( auto v ) { return v == 9; } ) )
		{
			XO_CHECK( i == 9 );
			++count;
		}
		XO_CHECK( count == 3 );
	}
}
