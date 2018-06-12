#pragma once

#include "xo/container/storage.h"
#include "xo/numerical/filter.h"
#include <fstream>

namespace xo
{
	inline float filter_test_function( float x )
	{
		float y = 0.0f;
		for ( int i = 1; i <= 8; ++i )
			y += std::sin( x * 2 * const_pi< float >() * i * i ) / i;
		return y;
	}

	void filter_test()
	{
		storage< float > sto;
		auto f1 = make_lowpass_butterworth_2nd_order( 0.05f ); // 50Hz
		auto f2 = make_lowpass_butterworth_2nd_order( 0.02f ); // 20Hz
		auto f3 = make_lowpass_butterworth_2nd_order( 0.01f ); // 10Hz
		auto f4 = make_lowpass_butterworth_2nd_order( 0.005f ); // 5Hz
		for ( float x = 0; x < 2; x += 0.001f ) // 1000Hz
		{
			auto f = sto.add_frame();
			auto s = filter_test_function( x );
			f[ "signal" ] = s;
			f[ "f1" ] = f1( s );
			f[ "f2" ] = f2( s );
			f[ "f3" ] = f3( s );
			f[ "f4" ] = f4( s );
		}
		
		std::ofstream( "X:/filter_test.txt" ) << sto;
	}
}
