#include "xo/numerical/random.h"
#include "xo/utility/irange.h"
#include "xo/numerical/regression.h"
#include "xo/system/profiler_config.h"

namespace xo
{
	void regression_test()
	{
		const int n = 500;

		linear_function< float > r1, r2, r3, r4, r5;

		for ( int t = 0; t < 1; ++t )
		{
			std::vector< float > y( n ), x( n );
			for ( index_t i = 0; i < n; ++i )
			{
				x[i] = float( i );
				y[i] = rand_norm< float >( float( i ), n / 10 );
			}
			auto xr = make_irange( 0, n );

			{
				XO_PROFILE_SCOPE( xo::profiler::instance(), "linear_regression" );
				r1 = linear_regression( x.begin(), x.end(), y.begin(), y.end() );
			}
			{
				XO_PROFILE_SCOPE( xo::profiler::instance(), "linear_regression_range" );
				r2 = linear_regression( xr.begin(), xr.end(), y.begin(), y.end() );
			}
			{
				XO_PROFILE_SCOPE( xo::profiler::instance(), "linear_regression_fixed_step" );
				r3 = linear_regression( y, 0.0f, 1.0f );
			}
			{
				XO_PROFILE_SCOPE( xo::profiler::instance(), "repeated_median_regression" );
				r4 = repeated_median_regression( x.begin(), x.end(), y.begin(), y.end() );
			}
			{
				XO_PROFILE_SCOPE( xo::profiler::instance(), "repeated_median_regression_range" );
				r5 = repeated_median_regression( xr.begin(), xr.end(), y.begin(), y.end() );
			}
		}
	}
}
