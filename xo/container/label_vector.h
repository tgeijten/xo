#pragma once

#include <unordered_map>
#include <vector>
#include "xo/utility/types.h"
#include "xo/system/assert.h"

namespace xo
{
	template< typename L >
	struct label_vector
	{
		label_vector( size_t channels = 0 ) : labels_( channels ) {}
		label_vector( std::initializer_list< L > labels ) : labels_( labels ) {}
		label_vector( const std::vector< L >& labels ) : labels_( labels ) {}
		~label_vector() {}

		index_t add( const L& label ) {
			xo_assert( find( label ) == no_index );
			labels_.push_back( label );
			return label_indices_[ label ] = labels_.size() - 1;
		}

		index_t find( const L& label ) const {
			auto it = label_indices_.find( label );
			return ( it != label_indices_.end() ) ? it->second : no_index;
		}

		index_t find_or_add( const L& label ) const {
			auto it = label_indices_.find( label );
			if ( it == label_indices_.end() ) {
				labels_.push_back( label );
				return label_indices_[ label ] = labels_.size() - 1;
			}
			else return it->second;
		}

		void resize( size_t s ) { xo_assert( s >= size() ); labels_.resize( s ); }

		void clear() { labels_.clear(); label_indices_.clear(); }

		size_t size() const { return labels_.size(); }

		const L& operator[]( index_t idx ) const { return labels_[ idx ]; }

		index_t set( index_t idx, const L& label ) {
			xo_assert( idx < size() && find( label ) == no_index );
			labels_[ idx ] = label;
			label_indices_[ label ] = labels_.size() - 1;
			return idx;
		}

		typename vector< L >::const_iterator begin() const { return labels_.begin(); }
		typename vector< L >::const_iterator end() const { return labels_.end(); }

	private:
		std::vector< L > labels_;
		std::unordered_map< L, index_t > label_indices_;
	};

	template<> struct label_vector< void >
	{
		label_vector( size_t channels = 0 ) : num_channels( channels ) { }
		size_t size() const { return num_channels; }
		index_t add() { return num_channels++; }
		index_t get_label( index_t idx ) const { return idx; }

	private:
		index_t num_channels;
	};
}
