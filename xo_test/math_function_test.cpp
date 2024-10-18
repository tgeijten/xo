#include "xo/system/test_case.h"
#include "xo/container/storage.h"
#include "xo/numerical/math.h"

#include <fstream>

namespace xo
{
	void math_function_test()
	{
		storage<double> sto;
		for ( double x = -1; x < 2.0; x += 0.01 )
		{
			auto f = sto.add_frame();
			f["smooth1"] = xo::smooth_clamped( x, 0.05, 0.8, 0.1, 0.5 );
			f["smooth2"] = xo::smooth_clamped( x, -0.5, 1.5, 0.25, 0.75 );
		}
		std::ofstream( "D:/smooth_clamp_test.txt" ) << sto;
	}

	//XO_TEST_CASE( xo_math_function ) { math_function_test(); }
}
