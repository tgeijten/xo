#pragma once

#include "math.h"
#include "xo/container/flat_map.h"
#include "xo/container/pair_type.h"

namespace xo
{
	template <typename T>
	T cubic_hermite( T x0, T y0, T dy0, T x1, T y1, T dy1, T x )
	{
		auto h = x1 - x0;
		auto t = ( x - x0 ) / h;
		auto t2 = t * t;
		auto t3 = t2 * t;
		auto h00 = 2 * t3 - 3 * t2 + 1;
		auto h10 = t3 - 2 * t2 + t;
		auto h01 = -2 * t3 + 3 * t2;
		auto h11 = t3 - t2;

		return h00 * y0 + h10 * ( h * dy0 ) + h01 * y1 + h11 * ( h * dy1 );
	}

	template< typename T >
	struct piecewise_cubic_spline
	{
		struct value_t { T y, dy; };
		using container_t = flat_map<T, value_t>;
		using point_t = typename container_t::value_type;

		piecewise_cubic_spline() : data_() {}

		void insert_point( T x, T y, T dy ) { data_[x] = value_t{ y, dy }; }

		T operator()( const T& x ) const
		{
			xo_assert( !data_.empty() );

			// handle special case with 1 data point
			if ( data_.size() == 1 )
				return data_.front().second.y;

			// normal case with >= 2 data points
			auto it1 = data_.lower_bound( x );
			if ( it1 == data_.end() )
				return extrapolate( data_.back(), x );
			else if ( it1 == data_.begin() )
				return extrapolate( data_.front(), x );

			auto it0 = it1 - 1;
			const auto& p0 = it0->second;
			const auto& p1 = it1->second;
			return cubic_hermite( it0->first, p0.y, p0.dy, it1->first, p1.y, p1.dy, x );
		}

		const container_t& data() const { return data_; }
		const point_t& point( index_t i ) const { return *( data_.begin() + i ); }
		size_t size() const { return data_.size(); }
		auto front() const { return data_.front(); }
		auto back() const { return data_.back(); }

	private:
		T extrapolate( const point_t& p, T x ) const { return p.second.y + p.second.dy * ( x - p.first ); }
		container_t data_;
	};
}
