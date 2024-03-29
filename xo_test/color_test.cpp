#include "xo/system/test_case.h"
#include "xo/utility/color.h"
#include <random>

namespace xo
{
	XO_TEST_CASE( xo_color_test )
	{
		std::default_random_engine re;
		std::uniform_int_distribution<uint32> rd( 0, 256 * 256 * 256 - 1 );
		std::uniform_int_distribution<uint32> rda( 0, 0xffffffff );

		for ( int i = 0; i < 100; ++i )
		{
			uint32 hex_col = rd( re );
			color col = color_from_hex_rgb( hex_col );
			auto hex_rgb2 = hex_rgb_from_color( col );
			XO_CHECK( hex_col == hex_rgb2 );

			uint32 hex_rgba = rda( re );
			color col_rgba = color_from_hex_rgba( hex_rgba );
			auto hex_rgba2 = hex_rgba_from_color( col_rgba );
			XO_CHECK( hex_rgba == hex_rgba2 );
		}
	}
}
