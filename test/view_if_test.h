#pragma once

#include "xo/container/view_if.h"

namespace xo
{
	void view_if_test()
	{
		std::vector<int> v{ 3, 1, 4, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9 };
		int count = 0;
		for ( auto& i : make_view_if( v, [&]( auto v ) { return v == 9; } ) )
		{
			XO_TEST( i == 9 );
			++count;
		}
		XO_TEST( count == 3 );
	}
}
