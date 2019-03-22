#pragma once

#include "math.h"
#include "xo/container/flat_map.h"

namespace xo
{
	template< typename T >
	struct piecewise_constant_function
	{
		using container_t = flat_map< T, T >;

		piecewise_constant_function() : data_() {}

		void insert_point( T x, T y ) { data_[ x ] = y; }

		T operator()( const T& x ) const
		{
			xo_assert( !data_.empty() );

			auto it = data_.upper_bound( x );
			if ( it == data_.end() )
				return data_.back().second;
			else if ( it == data_.begin() )
				return data_.front().second;
			else return (--it)->second;
		}

		const container_t& data() { return data_; }
		const std::pair< double, double >& point( index_t i ) { return *( data_.begin() + i ); }
		size_t size() const { return data_.size(); }

	private:
		container_t data_;
	};
}
