#pragma once

#include "math.h"
#include "xo/container/flat_map.h"

namespace xo
{
	template< typename T >
	struct piecewise_linear_function
	{
		using container_t = flat_map< T, T >;

		piecewise_linear_function() : data_() {}

		void insert_point( T x, T y ) { data_[ x ] = y; }

		T operator()( const T& x ) const
		{
			xo_assert( data_.size() >= 2 );

			auto it1 = data_.lower_bound( x );
			if ( it1 == data_.end() )
				--it1;
			else if ( it1 == data_.begin() )
				++it1;

			auto it0 = it1 - 1;
			return interpolate( x, it0->first, it0->second, it1->first, it1->second );
		}

		const container_t& data() { return data_; }
		const std::pair< double, double >& point( index_t i ) { return *( data_.begin() + i ); }
		size_t size() const { return data_.size(); }

	private:
		container_t data_;
	};
}
