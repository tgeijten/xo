#pragma once

#include "xo/container/vector_type.h"
#include "xo/container/pair_type.h"
#include "xo/string/string_type.h"
#include "container_tools.h"

namespace xo
{
	/// map in which elements are kept in insertion order
	/// it basically is a map interface on top of a vector of key-value pairs
	template< typename K, typename V >
	class indexed_map : private vector< pair<K, V> >
	{
	public:
		using key_type = K;
		using mapped_type = V;

		using typename vector< pair<K, V> >::value_type;
		using typename vector< pair<K, V> >::iterator;
		using typename vector< pair<K, V> >::const_iterator;

		using vector< pair<K, V> >::push_back;
		using vector< pair<K, V> >::emplace_back;
		using vector< pair<K, V> >::reserve;
		using vector< pair<K, V> >::clear;
		using vector< pair<K, V> >::empty;
		using vector< pair<K, V> >::size;
		using vector< pair<K, V> >::capacity;
		using vector< pair<K, V> >::begin;
		using vector< pair<K, V> >::end;
		using vector< pair<K, V> >::cbegin;
		using vector< pair<K, V> >::cend;
		using vector< pair<K, V> >::front;
		using vector< pair<K, V> >::back;
		using vector< pair<K, V> >::erase;
		using vector< pair<K, V> >::data;

		indexed_map() = default;
		indexed_map( std::initializer_list< pair<K, V> > l ) : vector< pair<K, V> >( l ) {}

		iterator find( const key_type& key, iterator it ) {
			for ( ; it != end(); ++it ) if ( it->first == key )	break; return it;
		}
		const_iterator find( const key_type& key, const_iterator it ) const {
			for ( ; it != end(); ++it ) if ( it->first == key )	break; return it;
		}
		iterator find( const key_type& key ) { return find( key, begin() ); }
		const_iterator find( const key_type& key ) const { return find( key, begin() ); }

		size_t count( const key_type& key ) const {
			return count_if( *this, [&]( const value_type& kvp ) { return kvp.first == key; } );
		}
		bool contains( const key_type& key ) const { return find( key ) != end(); }

		pair< iterator, bool > insert( value_type&& value ) {
			if ( auto it = find( key ); it != end() ) {
				it->second = std::move( value.second ); // replace existing item
				return { it, false };
			}
			else return { emplace_back( std::move( value ) ), true };
		}

		V& operator[]( const key_type& key ) {
			if ( auto it = find( key ); it != end() )
				return it->second;
			else return emplace_back( key, V{} ).second;
		}

		const V& operator[]( const key_type& key ) const {
			if ( auto it = find( key ); it != end() )
				return it->second;
			else xo_error( "Could not find key " + key );
		}

		/// access key-value pair through index
		value_type& operator[]( size_t idx ) { return vector< pair<K, V> >::operator[]( idx ); }
		const value_type& operator[]( size_t idx ) const { return vector< pair<K, V> >::operator[]( idx ); }
	};

	/// convert sorted_vector to string
	template< typename K, typename V > string to_str( const indexed_map<K, V>& v ) { return container_to_str( v ); }
}
