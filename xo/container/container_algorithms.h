#pragma once

#include <vector>
#include <algorithm>

#include "xo/system/assert.h"
#include "xo/xo_types.h"
#ifdef XO_COMP_MSVC
#   include <xstddef>
#endif

namespace xo
{
	template< typename I, typename T = typename std::iterator_traits< I >::value_type > T average( I b, I e, T v = T() ) {
		for ( auto i = b; i != e; ++i ) v += *i;
		return v / ( e - b );
	}

	template< typename C > typename C::value_type average( const C& cont )
	{ return average( std::cbegin( cont ), std::cend( cont ) ); }

	template < typename C, typename P > std::vector< index_t > sort_indices( const C& cont, P pred ) {
		std::vector< size_t > idx_vec( cont.size() );
		for ( index_t i = 0; i < cont.size(); ++i ) idx_vec[ i ] = i;
		std::sort( idx_vec.begin(), idx_vec.end(), [&]( size_t i1, size_t i2 ) { return pred( cont[ i1 ], cont[ i2 ] ); } );
		return idx_vec;
	}

	template < typename C > std::vector< index_t > sort_indices( const C& cont ) {
		return sort_indices( cont, std::less< typename C::value_type >() );
	}

	template< typename C > typename C::const_iterator min_element( const C& cont ) {
		return std::min_element( std::begin( cont ), std::end( cont ) );
	}

	template< typename C > typename C::const_iterator max_element( const C& cont ) {
		return std::max_element( std::begin( cont ), std::end( cont ) );
	}

	template< typename C > C sorted_copy( const C& cont ) { C res( cont ); std::sort( res.begin(), res.end() ); return res; }

	template< typename I > typename std::iterator_traits< I >::value_type median_slow( I b, I e ) {
		auto n = e - b;
		xo_assert( n > 0 );
		std::vector< typename std::remove_cv< typename std::iterator_traits< I >::value_type >::type > v( n / 2 + 1 );
		std::partial_sort_copy( b, e, v.begin(), v.end() );
		if ( n % 2 == 1 ) return v[ n / 2 ];
		else return ( v[ n / 2 ] + v[ n / 2 - 1 ] ) / std::iterator_traits< I >::value_type( 2 );
	}

	template< typename I > typename std::iterator_traits< I >::value_type median_non_const( I b, I e ) {
		xo_error_if( e <= b, "Invalid range" );
		auto n = e - b;
		auto h = n / 2;
		std::nth_element( b, b + h, e );
		if ( n % 2 == 1 )
			return *( b + h );
		else return ( *( b + h ) + *std::max_element( b, b + h ) ) / 2;
	}

	template< typename C > typename C::value_type median( C v ) {
		return median_non_const( std::begin( v ), std::end( v ) );
	}

	template< typename C > typename C::value_type median_non_const( C& v ) {
		return median_non_const( std::begin( v ), std::end( v ) );
	}

	template< typename C > typename C::value_type median_slow( const C& v ) {
		return median_slow( std::begin( v ), std::end( v ) );
	}

	template< typename C > typename C::value_type top_average( const C& vec, size_t count ) {
		// TODO: use nth_element for better performance
		C result( count );
		std::partial_sort_copy( vec.begin(), vec.end(), result.begin(), result.end() );
		return average( result );
	}
}
