#pragma once

#include <vector>

#include "xo/system/assert.h"
#include "xo/utility/types.h"

namespace xo
{
	/// find element in a container
	template< typename C > typename auto find( C& cont, const typename C::value_type& e )
	{ auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( *it == e ) break; return it; }

	/// find element in a container
	template< typename C, typename P > auto find( const C& cont, const typename C::value_type& e )
	{ auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( *it == e ) break; return it; }

	/// find element in a container
	template< typename C, typename P > auto find_if( C& cont, P pred )
	{ auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) break; return it; }

	/// find element in a container
	template< typename C, typename P > auto find_if( const C& cont, const P pred )
	{ auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) break; return it; }

	/// count element in a container
	template< typename C, typename P > size_t count( const C& cont, typename C::value_type& e )
	{ size_t c = 0; for ( auto it = std::begin( cont ); it != std::end( cont ); ++it ) c += size_t( *it == e ); return c; }

	/// count element in a container
	template< typename C, typename P > size_t count_if( const C& cont, P pred )
	{ size_t c = 0; for ( auto it = std::begin( cont ); it != std::end( cont ); ++it ) c += size_t( pred( *it ) ); return c; }

	/// copy elements of one container to another
	template< typename InIt, typename OutIt > OutIt copy( InIt ib, InIt ie, OutIt ob )
	{ for ( ; ib != ie; ++ib, ++ob ) *ob = *ib; }

	/// append two containers, currently requires C::reserve()
	template< typename C > C& append( C& c1, const C& c2 )
	{ c1.reserve( c1.size() + c2.size() ); for ( auto& e : c2 ) c1.emplace_back( e ); return c1; }

	template< typename T > index_t find_index( const vector< T >& vec, const T& val ) {
		auto it = std::find( vec.begin(), vec.end(), val );
		if ( it == vec.end() ) return no_index;
		else return it - vec.begin();
	}

	template< typename T > index_t back_index( const vector< T >& vec )
	{ xo_assert( vec.size() > 0 ) return vec.size() - 1; }

	template< typename T > std::ostream& operator<<( std::ostream& str, const vector< T >& vec ) {
		for ( auto it = vec.begin(); it != vec.end(); ++it ) {
			if ( it != vec.begin() ) str << "\t";
			str << *it;
		}
		return str << std::endl;
	}
}
