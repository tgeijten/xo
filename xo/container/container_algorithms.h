#pragma once

#include <vector>
#include <algorithm>

#include "xo/system/assert.h"
#include "xo/xo_types.h"
#include "xo/container/pair_type.h"
#include <cmath>

namespace xo
{
	template< typename It, typename T >
	T accumulate( It b, It e, T init ) {
		for ( ; b != e; ++b )
			init = init + *b;
		return init;
	}

	template< typename It, typename T, typename Op >
	T accumulate( It b, It e, T init, Op op ) {
		for ( ; b != e; ++b )
			init = op( init, *b );
		return init;
	}

	template< typename C, typename T >
	T accumulate( const C& cont, T init ) {
		return accumulate( std::cbegin( cont ), std::cend( cont ), init );
	}

	template< typename C, typename T, typename Op >
	T accumulate( const C& cont, T init, Op op ) {
		return accumulate( std::cbegin( cont ), std::cend( cont ), init, op );
	}

	template< typename It, typename T >
	T average( It b, It e, T v ) {
		xo_assert( b != e ); return xo::accumulate( b, e, v ) / ( e - b );
	}

	template< typename It, typename T, typename Op >
	T average( It b, It e, T v, Op op ) {
		xo_assert( b != e ); return xo::accumulate( b, e, v, op ) / ( e - b );
	}

	template< typename C >
	auto average( const C& cont ) {
		return average( std::cbegin( cont ), std::cend( cont ), C::value_type() );
	}

	template< typename C, typename T, typename Op >
	auto average( const C& cont, T init, Op op ) {
		return average( std::cbegin( cont ), std::cend( cont ), init, op );
	}

	template< typename It, typename T = typename std::iterator_traits<It>::value_type >
	auto mean_std( It b, It e, T v ) {
		xo_assert( b != e );
		auto mean = average( b, e, v );
		auto acc = decltype( mean * mean )();
		for ( auto i = b; i != e; ++i )
			acc += ( *i - mean ) * ( *i - mean );
		return std::make_pair( mean, std::sqrt( acc ) );
	}

	template< typename C > auto mean_std( const C& cont ) {
		return mean_std( std::cbegin( cont ), std::cend( cont ), C::value_type() );
	}

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
	template< typename C > auto min_value( const C& cont ) { return *min_element( cont ); }

	template< typename C > typename C::const_iterator max_element( const C& cont ) {
		return std::max_element( std::begin( cont ), std::end( cont ) );
	}
	template< typename C > auto max_value( const C& cont ) { return *max_element( cont ); }

	template< typename C > C sorted_copy( const C& cont ) { C res( cont ); std::sort( res.begin(), res.end() ); return res; }

	template< typename It > typename std::iterator_traits< It >::value_type median_slow( It b, It e ) {
		auto n = e - b;
		xo_assert( n > 0 );
		std::vector< typename std::remove_cv< typename std::iterator_traits< It >::value_type >::type > v( n / 2 + 1 );
		std::partial_sort_copy( b, e, v.begin(), v.end() );
		if ( n % 2 == 1 ) return v[ n / 2 ];
		else return ( v[ n / 2 ] + v[ n / 2 - 1 ] ) / std::iterator_traits< It >::value_type( 2 );
	}

	template< typename It > typename std::iterator_traits< It >::value_type median_non_const( It b, It e ) {
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
		// #todo: use nth_element for better performance
		C result( count );
		std::partial_sort_copy( vec.begin(), vec.end(), result.begin(), result.end() );
		return average( result );
	}
}
