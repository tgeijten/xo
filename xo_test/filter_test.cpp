#include "xo/container/storage.h"
#include "xo/numerical/filter.h"
#include "xo/numerical/constants.h"
#include <fstream>
#include "xo/system/test_case.h"
#include "xo/numerical/filter_1st_order.h"

namespace xo
{
	inline float filter_test_function( float x )
	{
		float y = 0.0f;
		for ( int i = 1; i <= 8; ++i )
			y += std::sin( x * 2 * constantsf::pi() * i * i ) / i;
		return y;
	}

	void filter_test()
	{
		auto pi2 = constantsf::two_pi();
		auto dt = 0.002f;
		storage< float > sto;
		auto bw100 = make_lowpass_butterworth_2nd_order( 100.0f * dt ); // 100Hz
		auto bw05 = make_lowpass_butterworth_2nd_order( 5.0f * dt ); // 5Hz
		auto fd100 = filter_1st_order<float>( pi2 * 100.0f );
		auto fd100a = filter_1st_order<float>( pi2 * 100.0f );
		auto fd05 = filter_1st_order<float>( pi2 * 5.0f );
		auto fd05a = filter_1st_order<float>( pi2 * 5.0f );
		for ( float x = 0; x < 0.5; x += dt ) // 1000Hz
		{
			auto f = sto.add_frame();
			auto s = filter_test_function( x );
			f["signal"] = s;
			f["bw100"] = bw100( s );
			//f["bw05"] = f4( s );
			f["fd100f"] = fd100.update_fast( s, dt );
			f["fd100a"] = fd100a.update_accurate( s, dt );
			//f["fd05f"] = fd05.update_fast( s, dt );
			//f["fd05a"] = fd05a.update_accurate( s, dt );
		}
		std::ofstream( "D:/filter_test.txt" ) << sto;
	}

	//XO_TEST_CASE( xo_filter_test ) { filter_test(); }
}
