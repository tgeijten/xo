#pragma once

#include "xo/system/assert.h"
#include "xo/xo_types.h"
#include <string>

namespace xo
{
	/// find element in a container
	template< typename C > auto find( C& cont, const typename C::value_type& e )
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

	/// find element in a container
	template< typename C, typename P > typename C::value_type& find_ref_if( C& cont, P pred )
	{ auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) return *it; xo_error( "Could not find element" ); }

	/// count element in a container
	template< typename C, typename P > size_t count( const C& cont, typename C::value_type& e )
	{ size_t c = 0; for ( auto it = std::begin( cont ); it != std::end( cont ); ++it ) c += size_t( *it == e ); return c; }

	/// count element in a container
	template< typename C, typename P > size_t count_if( const C& cont, P pred )
	{ size_t c = 0; for ( auto it = std::begin( cont ); it != std::end( cont ); ++it ) c += size_t( pred( *it ) ); return c; }

	/// copy elements of one container to another
	template< typename InIt, typename OutIt > OutIt copy( InIt ib, InIt ie, OutIt ob )
	{ for ( ; ib != ie; ++ib, ++ob ) *ob = *ib; }

	/// append a container to another
	template< typename C1, typename C2 > C1& append( C1& c1, const C2& c2 )
	{ c1.insert( c1.end(), c2.begin(), c2.end() ); return c1; }

	template< typename C > index_t find_index( const C& cont, const typename C::value_type& e ) {
		auto it = find( cont, e );
		if ( it == std::end( cont ) ) return no_index;
		else return it - std::begin( cont );
	}

	/// convert elements in a container to a string
	template< typename C > std::string container_to_str( const C& cont, const std::string& delim = " " ) {
		std::string str;
		for ( auto& e : cont ) {
			auto estr = to_str( e );
			if ( !estr.empty() ) {
				if ( !str.empty() ) str += delim;
				str += estr;
			}
		}
		return str;
	}

	/// get index of last element
	template< typename C > index_t back_index( const C& cont ) { return size( cont ) > 0 ? size( cont ) - 1 : no_index; }

	/// emplace back, return index
	template< typename C, typename... Args> index_t emplace_back_get_index( C& c, Args&&... a )
	{ c.emplace_back( std::forward< Args >( a )... ); return c.size() - 1; }
}
