#pragma once

#include <algorithm>
#include <vector>

namespace xo
{
	template< typename K, typename V >
	class flat_map
	{
	public:
		using container_t = typename std::vector< std::pair< K, V > >;
		using value_type = typename container_t::value_type;
		using iterator = typename container_t::iterator;
		using const_iterator = typename container_t::const_iterator;

		bool empty() const { return data_.empty(); }
		void clear() { data_.clear(); }
		size_t size() const { return data_.size(); }
		iterator begin() { return data_.begin(); }
		const_iterator begin() const { return data_.begin(); }
		const_iterator cbegin() const { return data_.begin(); }
		iterator end() { return data_.end(); }
		const_iterator end() const { return data_.end(); }
		const_iterator cend() const { return data_.end(); }
		iterator erase( iterator it ) { return data_.erase( it ); }

		iterator lower_bound( const K& key ) { return std::lower_bound( begin(), end(), key, compare_less ); }
		const_iterator lower_bound( const K& key ) const { return std::lower_bound( begin(), end(), key, compare_less ); }

		iterator find( const K& key ) {
			auto it = lower_bound( key );
			return ( it != end() && it->first == key ) ? it : end();
		}
		const_iterator find( const K& key ) const {
			auto it = lower_bound( key );
			return ( it != end() && it->first == key ) ? it : end();
		}

		size_t count( const K& key ) const { return std::count_if( cbegin(), cend(), [&]( const value_type& kvp ) { return kvp.first == key; } ); }
		bool has_key( const K& key ) const { return find( key ) != end(); }

		std::pair< iterator, bool > insert( const value_type& value ) {
			auto it = std::lower_bound( begin(), end(), value.key, compare_less );
			if ( it != end() && it->first == value.key )
				return make_pair( it, false );
			else return make_pair( data_.insert( it, value_type ), true );
		}

		V& operator[]( const K& key ) {
			auto it = std::lower_bound( begin(), end(), key, compare_less );
			if ( it != end() && it->first == key )
				return it->second;
			return data_.insert( it, std::make_pair( key, V() ) )->second;
		}

		const V& operator[]( const K& key ) const { return at( key ); }

		V& at( const K& key ) {
			auto it = find( key );
			xo_error_if( it == end(), "Could not find key: " + to_str( key ) );
			return it->second;
		}

		const V& at( const K& key ) const {
			auto it = find( key );
			xo_error_if( it == end(), "Could not find key: " + to_str( key ) );
			return it->second;
		}

	private:
		container_t data_;
		static bool compare_less( const value_type& kvp, const K& key ) { return kvp.first < key; }
		static bool compare_equal( const value_type& kvp, const K& key ) { return kvp.first == key; }
	};
}
