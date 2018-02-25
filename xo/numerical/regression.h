#pragma once

#include <numeric>

#include "math.h"
#include "polynomial.h"
#include "xo/container/container_algorithms.h"

namespace xo
{
	template< typename ItX, typename ItY >
	linear_function< typename ItY::value_type > linear_regression( ItX x_begin, ItX x_end, ItY y_begin, ItY y_end )
	{
		using T = typename ItY::value_type;
		auto n = x_end - x_begin;
		xo_assert_msg( n == y_end - y_begin, "Input ranges must be of equal size for x and y" );
		T avgx = average( x_begin, x_end );
		T avgY = average( y_begin, y_end );
		T num = 0.0;
		T den = 0.0;
		ItX itx = x_begin;
		ItY ity = y_begin;
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
		std::vector< T > slopes;
		slopes.reserve( cy.size() );
		for ( index_t i = 0; i < cy.size() - 1; ++i )
			slopes.push_back( cy[ i + 1 ] - cy[ i ] );
		auto medslope = median( slopes ) / x_step;
		auto medy = median( cy );
		auto medx = x_begin + x_step * ( cy.size() - 1 ) / 2;
		return linear_function< T >( medy - medslope * medx, medslope );
	}

	template< typename ItX, typename ItY >
	linear_function< typename std::remove_const< typename std::iterator_traits< ItY >::value_type >::type > repeated_median_regression( ItX xb, ItX xe, ItY yb, ItY ye )
	{
		// Implementation of the algorithm described in [Siegel 1982]
		using T = std::remove_const< std::iterator_traits< typename ItY >::value_type >::type;
		auto n = xe - xb;
		xo_assert_msg( n > 1 && n == ye - yb, "Input ranges must be > 1 and of equal size for x and y" );

		std::vector< T > sl1, sl2;
		sl1.reserve( n - 1 );
		sl2.reserve( n - 1 );

		for ( int i = 0; i < n; ++i )
		{
			sl1.clear();
			sl1.reserve( n - 1 );
			for ( int j = 0; j < i; ++j )
				sl1.emplace_back( ( *( yb + j ) - *( yb + i ) ) / ( *( xb + j ) - *( xb + i ) ) );
			for ( int j = i + 1; j < n; ++j )
				sl1.emplace_back( ( *( yb + j ) - *( yb + i ) ) / ( *( xb + j ) - *( xb + i ) ) );
			sl2.emplace_back( median( sl1 ) );
		}
		auto slope = median( sl2 );

		std::vector< T > intercepts( n );
		for ( int i = 0; i < n; ++i )
			intercepts[ i ] = *( yb + i ) - slope * *( xb + i );
		auto offset = median( intercepts );

		return linear_function< T >( offset, slope );
	}
}
