#include "xo/numerical/delayer.h"
#include "xo/container/storage.h"
#include "xo/system/log.h"
#include "xo/geometry/angle.h"
#include <cmath>
#include <fstream>
#include "xo/string/string_tools.h"
#include "xo/numerical/constants.h"

namespace xo
{
	inline float test_function( float x )
	{
		return std::sin( x * x * 16 * constantsf::pi() );
	}

	template< int N > void do_delay_test( float delay_inc, float dt, int times = 1 )
	{
		storage< float > sto;
		std::vector< delayer< float, N > > delvec1;
		for ( int i = 1; i <= times; ++i )
			delvec1.push_back( delayer< float, N >( i * delay_inc ) );

		std::vector< smooth_delayer< float, N > > delvec2;
		for ( int i = 1; i <= times; ++i )
			delvec2.emplace_back( i * delay_inc );

		for ( float t = 0; t < 1.0; t += dt )
		{
			auto func = test_function( t );
			auto frame = sto.add_frame();
			frame[ "time" ] = t;
			frame[ "func" ] = func;

			for ( int i = 0; i < times; ++i )
			{
				auto delay_amount = ( i + 1 ) * delay_inc;
				delvec1[ i ].add_sample( func, t == 0 ? 0.0f : dt );
				delvec2[ i ].add_sample( func, t == 0 ? 0.0f : dt );
				auto dd1 = delvec1[ i ].delayed_value();
				auto dd2 = delvec2[ i ].delayed_value();
				auto df = t > delay_amount ? test_function( t - delay_amount ) : 0;
				frame[ stringf( "delay%.2f", delay_amount ) ] = dd1;
				frame[ stringf( "sdelay%.2f", delay_amount ) ] = dd2;
				frame[ stringf( "fdelay%.2f", delay_amount ) ] = df;
				frame[ stringf( "error%.2f", delay_amount ) ] = dd1 - df;
				frame[ stringf( "serror%.2f", delay_amount ) ] = dd2 - df;
			}
		}
		std::ofstream fs( stringf( "X:/delay_test_%d_%.3f_%.3f.txt", N, delay_inc, dt ) );
		fs << sto;
	}

	void delay_test()
	{
		do_delay_test< 2 >( 0.01f, 0.002f, 5 );
		do_delay_test< 3 >( 0.01f, 0.002f, 5 );
		do_delay_test< 4 >( 0.01f, 0.002f, 5 );
	}
}
