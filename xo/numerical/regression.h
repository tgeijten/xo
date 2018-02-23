#pragma once

#include <numeric>

#include "math.h"
#include "polynomial.h"
#include "xo/container/container_algorithms.h"

namespace xo
{
	template< typename IterX, typename IterY >
	linear_function< typename IterY::value_type > linear_regression( IterX x_begin, IterX x_end, IterY y_begin, IterY y_end )
	{
		using T = typename IterY::value_type;
		auto n = x_end - x_begin;
		xo_assert_msg( n == y_end - y_begin, "Input ranges must be of equal size for x and y" );
		T avgx = average( x_begin, x_end );
		T avgY = average( y_begin, y_end );
		T num = 0.0;
		T den = 0.0;
		IterX itx = x_begin;
		IterY ity = y_begin;
		for (; itx != x_end; ++itx, ++ity )
		{
			num += ( *itx - avgx ) * ( *ity - avgY );
			den += ( *itx - avgx ) * ( *itx - avgx );
		}
		T slope = num / den;
		return linear_function< T >( avgY - slope * avgx, slope );
	}

	template< typename CY >
	linear_function< typename CY::value_type > linear_regression( const CY& cy, typename CY::value_type x_begin, typename CY::value_type x_step )
	{
		using T = typename CY::value_type;
		auto n = cy.size();
		T avgx = x_begin + ( T( n - 1 ) * x_step ) / 2;
		T avgy = average( std::begin( cy ), std::end( cy ) );
		T num = 0.0;
		T xval = x_begin;
		for ( auto ity = std::begin( cy ); ity != std::end( cy ); ++ity, xval += x_step )
			num += ( xval - avgx ) * ( *ity - avgy );
		T x_hsiz = T( n - 1 ) * x_step / 2;
		T den = x_hsiz * ( x_hsiz + x_step ) * ( 2 * x_hsiz + x_step ) / ( 3 * x_step ); // analytical solution for den with fixed steps
		T slope = num / den;
		return linear_function< T >( avgy - slope * avgx, slope );
	}

	template< typename C1, typename C2 >
	auto linear_regression( const C1& cx, const C2& cy )
	{ return linear_regression( std::begin( cx ), std::end( cx ), std::begin( cy ), std::end( cy ) ); }

	template< typename CY >
	linear_function< typename CY::value_type > linear_median_regression( const CY& cy, typename CY::value_type x_begin, typename CY::value_type x_step )
	{
		using T = typename CY::value_type;
		xo_assert( cy.size() >= 2 );
		std::vector< T > slopes, values;
		slopes.reserve( cy.size() );
		for ( index_t i = 0; i < cy.size() - 1; ++i )
			slopes.push_back( cy[ i + 1 ] - cy[ i ] );
		auto medslope = median( slopes ) / x_step;
		auto medy = median( cy );
		auto medx = x_begin + x_step * ( cy.size() - 1 ) / 2;
		return linear_function< T >( medy - medslope * medx, medslope );
	}
}
