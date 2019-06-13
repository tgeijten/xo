#pragma once

#include <functional>
#include <memory>
#include "xo/numerical/math.h"

namespace xo
{
	template< typename T, size_t N >
	struct regular_piecewise_linear_function
	{
		regular_piecewise_linear_function( T x_min, T x_max, typename std::function< T( T ) > fn ) :
			x_base( x_min ),
			x_step( ( x_max - x_min ) / T( N - 1 ) )
		{
			static_assert( N >= 2, "regular_piecewise_linear_function must have at least 2 data points" );
			for ( int i = 0; i < N; ++i )
				data[ i ] = fn( x_base + i * x_step );
		}

		T operator()( const T& x )
		{
			T x_idx = ( x - x_base ) / x_step;
			int xlb = int( floor( x_idx ) ); // find lower bound index of x
			clamp( xlb, 0, int( N ) - 2 ); // clamp index only (for extrapolation)
			T wub = x_idx - xlb;
			return ( T( 1 ) - wub ) * data[ xlb ] + wub * data[ xlb + 1 ];
		}

	private:
		T x_base;
		T x_step;
		std::array< T, N > data;
	};
}
