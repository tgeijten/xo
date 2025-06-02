#pragma once

#include "xo/system/assert.h"
#include "xo/xo_types.h"
#include <iterator> // #todo get rid of this header, used by std::begin / std::end
#include <vector>

namespace xo
{
	/// apply function to each element in a container
	template< typename C, typename F >
	void for_each( const C& cont, F fn ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) fn( *it );
	}

	/// apply function to each element in a container
	template< typename C, typename F >
	void for_each( C& cont, F fn ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) fn( *it );
	}

	/// find element in a container
	template< typename C > auto find( C& cont, const typename C::value_type& e ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( *it == e ) break; return it;
	}

	/// find element in a container
	template< typename C > auto find( const C& cont, const typename C::value_type& e ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( *it == e ) break; return it;
	}

	/// find the index of an element in an array-type container
	template< typename C > index_t find_index( const C& cont, const typename C::value_type& e, index_t idx = 0 ) {
		for ( ; idx < std::size( cont ); ++idx ) if ( cont[idx] == e ) return idx; return no_index;
	}

	/// find element in a container, returns iterator
	template< typename C, typename P > auto find_if( C& cont, P pred ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) break; return it;
	}

	/// find element in a container, returns iterator
	template< typename C, typename P > auto find_if( const C& cont, const P pred ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) break; return it;
	}

	/// find index to element in a container
	template< typename C, typename P > index_t find_index_if( const C& cont, const P pred, index_t idx = 0 ) {
		for ( ; idx < std::size( cont ); ++idx ) if ( pred( cont[idx] ) ) return idx; return no_index;
	}

	/// find reference to element in a container, throw if not found
	template< typename C, typename P > const auto& find_ref_if( const C& cont, P pred ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) return *it; xo_error( "Could not find element" );
	}

	/// find reference to element in a container, throw if not found
	template< typename C, typename P > auto& find_ref_if( C& cont, P pred ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) return *it; xo_error( "Could not find element" );
	}

	/// count element in a container
	template< typename C > size_t count( const C& cont, typename C::value_type& e ) {
		size_t c = 0; for ( auto it = std::begin( cont ); it != std::end( cont ); ++it ) c += size_t( *it == e ); return c;
	}

	/// count element in a container
	template< typename C, typename P > size_t count_if( const C& cont, P pred ) {
		size_t c = 0; for ( auto it = std::begin( cont ); it != std::end( cont ); ++it ) c += size_t( pred( *it ) ); return c;
	}

	/// checks if any element that meets pred is in a container
	template< typename C, typename P > auto any_of( const C& cont, const P pred ) {
		return find_if( cont, pred ) != cont.end();
	}

	/// checks if no elements that meet pred is in a container
	template< typename C, typename P > auto none_of( const C& cont, const P pred ) {
		return find_if( cont, pred ) == cont.end();
	}

	/// check if container contains element
	template< typename C > bool contains( const C& cont, const typename C::value_type& e ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( *it == e ) return true; return false;
	}

	/// check if container contains element
	template< typename C, typename P > bool contains_if( const C& cont, P pred ) {
		auto it = std::begin( cont ); for ( ; it != std::end( cont ); ++it ) if ( pred( *it ) ) return true; return false;
	}

	/// copy elements based on input range (no range checking)
	template< typename InIt, typename OutIt > OutIt copy( InIt ib, InIt ie, OutIt ob ) {
		for ( ; ib != ie; ++ib, ++ob ) *ob = *ib; return ob;
	}

	/// copy elements of one container to another (no range checking)
	template< typename InC, typename OutIt > OutIt copy( const InC& cont, OutIt ob ) {
		return xo::copy( std::begin( cont ), std::end( cont ), ob );
	}

	/// copy elements based on output range (no range checking)
	template< typename InIt, typename OutIt > InIt copy_to_range( InIt ib, OutIt ob, OutIt oe ) {
		for ( ; ob != oe; ++ib, ++ob ) *ob = *ib; return ib;
	}

	/// copy elements to output container (no range checking)
	template< typename InIt, typename OutC > InIt copy_to_range( InIt it, OutC& cont ) {
		return copy_to_range( it, std::begin( cont ), std::end( cont ) );
	}

	/// copy elements of one container to another, explicitly casting to target value type (no range checking)
	template< typename InIt, typename OutIt > OutIt copy_cast( InIt ib, InIt ie, OutIt ob ) {
		for ( ; ib != ie; ++ib, ++ob )
			*ob = static_cast<typename std::iterator_traits<OutIt>::value_type>( *ib );
		return ob;
	}

	/// copy elements of one container to another (with range checking)
	template< typename InC, typename OutC > void copy_container( const InC& source, OutC& target ) {
		xo_assert( std::size( source ) <= std::size( target ) );
		copy( std::begin( source ), std::end( source ), std::begin( target ) );
	}

	/// copy elements of one container to another, explicitly casting to target value type (with range checking)
	template< typename InC, typename OutC > void copy_cast( const InC& source, OutC& target ) {
		xo_assert( std::size( source ) <= std::size( target ) );
		copy_cast( std::begin( source ), std::end( source ), std::begin( target ) );
	}

	/// append container c2 to container c1
	template< typename C1, typename C2 > C1& append( C1& c1, const C2& c2 ) {
		c1.insert( c1.end(), c2.begin(), c2.end() ); return c1;
	}

	/// append c1 to c2 while skipping duplicates
	template< typename C1, typename C2 > C1& merge( C1& c1, const C2& c2 ) {
		for ( auto&& e : c2 ) if ( !contains( c1, e ) ) c1.emplace_back( e ); return c1;
	}

	/// get index of last element
	template< typename C > index_t back_index( const C& cont ) { return size( cont ) > 0 ? size( cont ) - 1 : no_index; }

	/// get index of element in container
	template< typename E > index_t index_of( const E& elem, const std::vector<E>& vec ) { return index_t( &elem - &vec[0] ); }

	/// emplace back, return index
	template< typename C, typename... Args> index_t emplace_back_get_index( C& c, Args&&... a ) {
		c.emplace_back( std::forward< Args >( a )... ); return c.size() - 1;
	}
}
