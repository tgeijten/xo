#pragma once

#include <algorithm>
#include "xo/container/vector_type.h"
#include "xo/container/pair_type.h"
#include "xo/system/assert.h"
#include "xo/string/string_cast.h"

namespace xo
{
	template< typename K >
	class flat_set
	{
	public:
		using key_type = K;
		using container_t = typename xo::vector< key_type >;
		using iterator = typename container_t::iterator;
		using reverse_iterator = typename container_t::reverse_iterator;
		using const_iterator = typename container_t::const_iterator;
		using const_reverse_iterator = typename container_t::const_reverse_iterator;

		flat_set() = default;
		~flat_set() = default;
		flat_set( const flat_set& other ) = default;
		flat_set( flat_set&& other ) = default;
		flat_set( std::initializer_list< key_type > l ) : data_( l ) { sort(); }
		flat_set& operator=( const flat_set& other ) = default;
		flat_set& operator=( flat_set&& other ) = default;

		bool empty() const { return data_.empty(); }
		void clear() { data_.clear(); }
		size_t size() const { return data_.size(); }

		iterator begin() { return data_.begin(); }
		const_iterator begin() const { return data_.begin(); }
		const_iterator cbegin() const { return data_.cbegin(); }
		reverse_iterator rbegin() { return data_.rbegin(); }
		const_reverse_iterator crbegin() const { return data_.crbegin(); }

		iterator end() { return data_.end(); }
		const_iterator end() const { return data_.end(); }
		const_iterator cend() const { return data_.cend(); }
		reverse_iterator rend() { return data_.rend(); }
		const_reverse_iterator crend() const { return data_.crend(); }

		const key_type& front() const { return data_.front(); }
		key_type& front() { return data_.front(); }

		const key_type& back() const { return data_.back(); }
		key_type& back() { return data_.back(); }

		iterator erase( iterator it ) { return data_.erase( it ); }
		size_t erase( const key_type& key ) {
			if ( auto it = find( key ); it != end() )
				return data_.erase( it ), 1;
			else return 0;
		}

		iterator lower_bound( const key_type& key ) {
			return std::lower_bound( begin(), end(), key, [&]( const key_type& kvp, const key_type& key ) { return kvp < key; } );
		}
		const_iterator lower_bound( const key_type& key ) const {
			return std::lower_bound( begin(), end(), key, [&]( const key_type& kvp, const key_type& key ) { return kvp < key; } );
		}

		iterator upper_bound( const key_type& key ) {
			return std::upper_bound( begin(), end(), key, [&]( const key_type& key, const key_type& kvp ) { return key < kvp; } );
		}
		const_iterator upper_bound( const key_type& key ) const {
			return std::upper_bound( begin(), end(), key, [&]( const key_type& key, const key_type& kvp ) { return key < kvp; } );
		}

		iterator find( const key_type& key ) {
			auto it = lower_bound( key );
			return ( it != end() && *it == key ) ? it : end();
		}
		const_iterator find( const key_type& key ) const {
			auto it = lower_bound( key );
			return ( it != end() && *it == key ) ? it : end();
		}

		size_t count( const key_type& key ) const {
			return std::count_if( cbegin(), cend(), [&]( const key_type& kvp ) { return kvp == key; } );
		}
		bool contains( const key_type& key ) const { return find( key ) != end(); }

		pair< iterator, bool > insert( const key_type& value ) {
			auto it = lower_bound( value );
			if ( it != end() && *it == value )
				return make_pair( it, false );
			else return make_pair( data_.insert( it, value ), true );
		}

		pair< iterator, bool > insert( key_type&& value ) {
			auto it = lower_bound( value );
			if ( it != end() && *it == value )
				return make_pair( it, false );
			else return make_pair( data_.insert( it, std::move( value ) ), true );
		}

		void reserve( size_t s ) { data_.reserve( s ); }

		const container_t& container() const { return data_; }

	private:
		container_t data_;
		void sort() {
			std::sort( data_.begin(), data_.end(), [&]( const key_type& a, const key_type& b ) { return a < b; } );
		}
	};

	/// convert flat_set to string
	template< typename K >
	string to_str( const flat_set<K>& m ) {
		if ( m.size() == 1 )
			return to_str( m.front() );
		string s = "[ ";
		for ( const auto& key : m )
			s += to_str( key ) + " ";
		s += "]";
		return s;
	}
}
