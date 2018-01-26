#pragma once

#include <vector>
#include <algorithm>

#include "xo/system/assert.h"
#include "xo/utility/types.h"

namespace xo
{
	template< typename I > typename std::iterator_traits< I >::value_type average( I b, I e ) {
		auto v = std::iterator_traits< I >::value_type();
		for ( auto i = b; i != e; ++i ) v += *i;
		return v / ( e - b );
	}

	template< typename C > typename C::value_type average( const C& cont )
	{ return average( std::cbegin( cont ), std::cend( cont ) ); }

	/// average value of container elements
	//template< typename I > typename std::iterator_traits< I >::value_type average( I b, I e )
	//{
	//	auto v = std::iterator_traits< I >::value_type(); for ( ; b != e; ++b ) v += *b; return v / ( e - b );
	//}

	//template< typename C > typename C::value_type average( const C& cont )
	//{
	//	return average( std::cbegin( cont ), std::cend( cont ) );
	//}

	template < typename C > std::vector< index_t > sort_indices( const C& cont ) {
		std::vector< size_t > idx_vec( cont.size() );
		for ( index_t i = 0; i < cont.size(); ++i ) idx_vec[ i ] = i;
		std::sort( idx_vec.begin(), idx_vec.end(), [&cont]( size_t i1, size_t i2 ) { return cont[ i1 ] < cont[ i2 ]; } );
		return idx_vec;
	}

	template< typename C > C sorted_copy( const C& cont ) { C res( cont ); std::sort( res.begin(), res.end() ); return res; }

	template< typename I > typename std::iterator_traits< I >::value_type median( I b, I e ) {
		auto s = e - b;
		xo_assert( s > 0 );
		std::vector< std::remove_cv< std::iterator_traits< I >::value_type >::type > v( e - b );
		std::copy( b, e, v.begin() );
		if ( s % 2 == 1 ) return v[ s / 2 ];
		else return ( v[ s / 2 ] + v[ s / 2 - 1 ] ) / std::iterator_traits< I >::value_type( 2 );
	}

	template< typename C > typename C::value_type median( const C& v ) {
		return median( std::cbegin( v ), std::cend( v ) );
	}

	template< typename C > typename C::value_type top_average( const C& vec, size_t count ) {
		C result( count );
		std::partial_sort_copy( vec.begin(), vec.end(), result.begin(), result.end() );
		return average( result );
	}
}
