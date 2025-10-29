#pragma once

#include "xo/xo_types.h"
#include <unordered_map>
#include <vector>
#include "xo/system/assert.h"

namespace xo
{
	template< typename T >
	struct index_map
	{
		index_map( size_t channels = 0 ) : values_( channels ) {}
		index_map( std::initializer_list<T> values ) : values_( values ) {}
		index_map( const std::vector<T>& values ) : values_( values ) {}
		~index_map() {}

		index_t add( const T& value ) {
			xo_assert( try_find( value ) == no_index );
			values_.push_back( value );
			return value_indices_[value] = values_.size() - 1;
		}

		index_t try_find( const T& value ) const {
			auto it = value_indices_.find( value );
			return ( it != value_indices_.end() ) ? it->second : no_index;
		}

		index_t find( const T& value ) const {
			auto it = value_indices_.find( value );
			xo_error_if( it == value_indices_.end(), "Could not find value: " + value );
			return it->second;
		}

		index_t find_or_add( const T& value ) const {
			auto it = value_indices_.find( value );
			if ( it == value_indices_.end() ) {
				values_.push_back( value );
				return value_indices_[value] = values_.size() - 1;
			}
			else return it->second;
		}

		void resize( size_t s ) { xo_assert( s >= size() ); values_.resize( s ); }
		void reserve( size_t s ) { values_.reserve( s ); }
		void clear() { values_.clear(); value_indices_.clear(); }

		size_t size() const { return values_.size(); }

		const T& operator[]( index_t idx ) const { return values_[idx]; }

		index_t set( index_t idx, const T& value ) {
			xo_assert( idx < size() && try_find( value ) == no_index );
			values_[idx] = value;
			value_indices_[value] = idx;
			return idx;
		}

		typename std::vector<T>::const_iterator begin() const { return values_.begin(); }
		typename std::vector<T>::const_iterator end() const { return values_.end(); }

	private:
		std::vector<T> values_;
		std::unordered_map<T, index_t> value_indices_;
	};

	template<> struct index_map< index_t >
	{
		index_map( size_t channels = 0 ) : num_channels( channels ) { }
		size_t size() const { return num_channels; }
		index_t add( index_t i ) { return num_channels++; }
		index_t get_value( index_t idx ) const { return idx; }

	private:
		index_t num_channels;
	};
}
