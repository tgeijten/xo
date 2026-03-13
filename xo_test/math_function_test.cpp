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
			f["smooth"] = xo::smooth_clamped( x, 0.0, 1.0, 0.1, 0.9 );
			f["soft1"] = xo::soft_clamped( x, 0.0, 1.0, 0.1 );
			f["soft2"] = xo::soft_clamped( x, 0.0, 1.0, 0.1, 0.9 );
			f["smooth_asym"] = xo::smooth_clamped( x, -0.1, 0.8, 0.2, 0.5 );
			f["soft_asym"] = xo::soft_clamped( x, -0.1, 0.8, 0.2, 0.5 );
		}
		std::ofstream( "D:/smooth_clamp_test.txt" ) << sto;
	}

	//XO_TEST_CASE( xo_math_function ) { math_function_test(); }
}
