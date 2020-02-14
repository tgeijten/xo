#pragma once

#include <algorithm>
#include "xo/container/vector_type.h"
#include "xo/container/pair_type.h"
#include "xo/system/assert.h"
#include "xo/string/string_cast.h"

namespace xo
{
	template< typename K, typename V >
	class flat_map
	{
	public:
		using key_type = K;
		using mapped_type = V;
		using value_type = typename xo::pair< K, V >;
		using container_t = typename xo::vector< value_type >;
		using iterator = typename container_t::iterator;
		using reverse_iterator = typename container_t::reverse_iterator;
		using const_iterator = typename container_t::const_iterator;
		using const_reverse_iterator = typename container_t::const_reverse_iterator;

		flat_map() : data_() {}
		flat_map( const flat_map& other ) : data_( other.data_ ) {}
		flat_map( flat_map&& other ) : data_( std::move( other.data_ ) ) {}
		flat_map( std::initializer_list< value_type > l ) : data_( l ) { sort(); }
		flat_map& operator=( const flat_map& other ) { data_ = other.data_; return *this; }
		flat_map& operator=( flat_map&& other ) { data_ = std::move( other.data_ ); return *this; }

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

		const value_type& front() const { return data_.front(); }
		value_type& front() { return data_.front(); }

		const value_type& back() const { return data_.back(); }
		value_type& back() { return data_.back(); }

		iterator erase( iterator it ) { return data_.erase( it ); }
		size_t erase( const key_type& key ) {
			if ( auto it = find( key ); it != end() )
				return data_.erase( it ), 1;
			else return 0;
		}

		iterator lower_bound( const key_type& key ) {
			return std::lower_bound( begin(), end(), key, [&]( const value_type& kvp, const key_type& key ) { return kvp.first < key; } );
		}
		const_iterator lower_bound( const key_type& key ) const {
			return std::lower_bound( begin(), end(), key, [&]( const value_type& kvp, const key_type& key ) { return kvp.first < key; } );
		}

		iterator upper_bound( const key_type& key ) {
			return std::upper_bound( begin(), end(), key, [&]( const key_type& key, const value_type& kvp ) { return key < kvp.first; } );
		}
		const_iterator upper_bound( const key_type& key ) const {
			return std::upper_bound( begin(), end(), key, [&]( const key_type& key, const value_type& kvp ) { return key < kvp.first; } );
		}

		iterator find( const key_type& key ) {
			auto it = lower_bound( key );
			return ( it != end() && it->first == key ) ? it : end();
		}
		const_iterator find( const key_type& key ) const {
			auto it = lower_bound( key );
			return ( it != end() && it->first == key ) ? it : end();
		}

		size_t count( const key_type& key ) const {
			return std::count_if( cbegin(), cend(), [&]( const value_type& kvp ) { return kvp.first == key; } );
		}
		bool contains( const key_type& key ) const { return find( key ) != end(); }

		pair< iterator, bool > insert( const value_type& value ) {
			auto it = lower_bound( value.first );
			if ( it != end() && it->first == value.first ) {
				it->second = value.second; // replace existing item
				return make_pair( it, false );
			}
			else return make_pair( data_.insert( it, value ), true );
		}

		pair< iterator, bool > insert( value_type&& value ) {
			auto it = lower_bound( value.first );
			if ( it != end() && it->first == value.first ) {
				it->second = std::move( value.second ); // replace existing item
				return make_pair( it, false );
			}
			else return make_pair( data_.insert( it, std::move( value ) ), true );
		}

		V& operator[]( const key_type& key ) {
			auto it = lower_bound( key );
			if ( it != end() && it->first == key )
				return it->second;
			return data_.insert( it, std::make_pair( key, V() ) )->second;
		}

		const V& operator[]( const key_type& key ) const { return at( key ); }

		V& at( const key_type& key ) {
			auto it = find( key );
			xo_error_if( it == end(), "Could not find key: " + to_str( key ) );
			return it->second;
		}

		const V& at( const key_type& key ) const {
			auto it = find( key );
			xo_error_if( it == end(), "Could not find key: " + to_str( key ) );
			return it->second;
		}

		void reserve( size_t s ) { data_.reserve( s ); }

	private:
		container_t data_;
		void sort() {
			std::sort( data_.begin(), data_.end(), [&]( const value_type& a, const value_type& b ) { return a.first < b.first; } );
		}
	};

	/// convert flat_map to string
	template< typename K, typename V >
	string to_str( const flat_map<K, V>& m ) {
		string s = "{ ";
		for ( const auto& [key, value] : m )
			s += to_str( key ) + " = " + to_str( value ) + " ";
		s += "}";
		return s;
	}
}
