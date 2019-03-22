#pragma once

#include <functional>
#include <memory>
#include "xo/numerical/math.h"

namespace xo
{
	template< typename T >
	struct regular_piecewise_linear_function
	{
		regular_piecewise_linear_function( T x_min, T x_max, int intervals, std::function< double( double ) > fn ) :
		x_base( x_min ),
		x_step( ( x_max - x_min ) / intervals ),
		points( intervals + 1 ),
		data( new T[ intervals + 1 ] )
		{
			// init data
			for ( int i = 0; i < points; ++i )
				data[ i ] = fn( x_base + i * x_step );
		}

		T operator()( const T& x )
		{
			// find lower bound index of x
			T x_idx = ( x - x_base ) / x_step;
			int xlb = int( floor( x_idx ) );
			clamp( xlb, 0, points - 2 );
			T wub = x_idx - xlb;
			return ( T(1) - wub ) * data[ xlb ] + wub * data[ xlb + 1 ];
		}

	private:
		T x_base;
		T x_step;
		int points;
		std::unique_ptr< T[] > data;
	};
}
