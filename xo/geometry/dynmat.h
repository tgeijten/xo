#pragma once

#include "dynvec.h"
#include "xo/container/dynarray.h"

namespace xo
{
	template< typename T >
	struct dynmat
	{
		using value_type = T;
		using iterator = typename dynarray< T >::iterator;
		using const_iterator = typename dynarray< T >::const_iterator;

		dynmat() : data_(), cols_() {}
		dynmat( size_t col, size_t row, const T& value = T() ) : data_( row * col, value ), cols_( col ) {}

		const T& operator()( index_t col, index_t row ) const { return data_[ row * cols_ + col ]; }
		T& operator()( index_t col, index_t row ) { return data_[ row * cols_ + col ]; }

		void resize( size_t newcols, size_t newrows ) {
			dynmat<T> newmat( newcols, newrows, T() );
			auto rmin = xo::min( rows(), newrows );
			auto cmin = xo::min( cols(), newcols );
			for ( auto r = 0; r < rmin; ++r )
				for ( auto c = 0; c < cmin; ++c )
					newmat( c, r ) = (*this)( c, r );
			*this = std::move( newmat );
		}

		bool empty() const { return data_.empty(); }
		void clear() { data_.clear(); cols_ = 0; }

		size_t cols() const { return cols_; }
		size_t rows() const { return empty() ? 0 : data_.size() / cols_; }

		iterator begin() { return data_.begin(); }
		const_iterator begin() const { return data_.begin(); }
		const_iterator cbegin() const { return data_.cbegin(); }

		iterator end() { return data_.end(); }
		const_iterator end() const { return data_.end(); }
		const_iterator cend() const { return data_.cend(); }

	private:
		size_t cols_;
		dynarray< T > data_;
	};

	template< typename T > dynvec< T > operator*( const dynmat< T >& m, const dynvec< T >& v )
	{
		flut_assert( m.cols() == v.size() );
		dynvec< T > r( m.rows() );
		for ( index_t row = 0; row < m.rows(); ++row )
		{
			T sum = 0.0;
			for ( index_t col = 0; col < m.cols(); ++col )
				sum += m( row, col ) * v[ col ];
			r[ row ] = sum;
		}
		return r;
	}
}
