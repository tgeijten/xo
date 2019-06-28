#pragma once

#include "xo/xo_types.h"
#include "xo/container/flat_map.h"
#include "xo/numerical/constants.h"
#include "xo/utility/optional.h"
#include <vector>

namespace xo
{
	template< typename T, typename I = index_t >
	class indexed_set
	{
	public:
		indexed_set() {}
		~indexed_set() {}

		const T& operator[]( I idx ) const { return values_[ idx ]; }

		optional<I> try_get( const T& value ) const {
			if ( auto it = indices_.find( value ); it != indices_.end() )
				return it->second;
			else return xo::optional<I>();
		}

		I get_or_add( const T& value ) {
			auto it = indices_.find( value );
			if ( it == indices_.end() ) { // add new
				values_.push_back( value );
				auto index = values_.size() - 1;
				// check if the index is valid (must be SMALLER than max to avoid sentinel)
				xo_error_if( index >= size_t( constants<I>::max() ), "index_set overflow" );
				return indices_[ value ] = static_cast<I>( index );
			}
			else return it->second; // get existing
		}

		size_t size() const { return values_.size(); }

		void clear() { values_.clear(); indices_.clear(); }

	private:
		std::vector< T > values_;
		flat_map< T, I > indices_;
	};
}
