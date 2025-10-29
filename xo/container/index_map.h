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
		index_map( size_t channels = 0 ) : labels_( channels ) {}
		index_map( std::initializer_list<T> labels ) : labels_( labels ) {}
		index_map( const std::vector<T>& labels ) : labels_( labels ) {}
		~index_map() {}

		index_t add( const T& label ) {
			xo_assert( find( label ) == no_index );
			labels_.push_back( label );
			return label_indices_[label] = labels_.size() - 1;
		}

		index_t find( const T& label ) const {
			auto it = label_indices_.find( label );
			return ( it != label_indices_.end() ) ? it->second : no_index;
		}

		index_t find_or_throw( const T& label ) const {
			auto it = label_indices_.find( label );
			xo_error_if( it == label_indices_.end(), "Could not find label: " + label );
			return it->second;
		}

		index_t find_or_add( const T& label ) const {
			auto it = label_indices_.find( label );
			if ( it == label_indices_.end() ) {
				labels_.push_back( label );
				return label_indices_[label] = labels_.size() - 1;
			}
			else return it->second;
		}

		void resize( size_t s ) { xo_assert( s >= size() ); labels_.resize( s ); }
		void reserve( size_t s ) { labels_.reserve( s ); }
		void clear() { labels_.clear(); label_indices_.clear(); }

		size_t size() const { return labels_.size(); }

		const T& operator[]( index_t idx ) const { return labels_[idx]; }

		index_t set( index_t idx, const T& label ) {
			xo_assert( idx < size() && find( label ) == no_index );
			labels_[idx] = label;
			label_indices_[label] = idx;
			return idx;
		}

		typename std::vector<T>::const_iterator begin() const { return labels_.begin(); }
		typename std::vector<T>::const_iterator end() const { return labels_.end(); }

	private:
		std::vector<T> labels_;
		std::unordered_map<T, index_t> label_indices_;
	};

	template<> struct index_map< void >
	{
		index_map( size_t channels = 0 ) : num_channels( channels ) { }
		size_t size() const { return num_channels; }
		index_t add() { return num_channels++; }
		index_t get_label( index_t idx ) const { return idx; }

	private:
		index_t num_channels;
	};
}
