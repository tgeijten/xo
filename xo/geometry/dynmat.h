#pragma once

#include "dynvec.h"

namespace xo
{
	template< typename T >
	struct dynmat
	{
		using value_type = T;
		using iterator = dynarray< T >::iterator;
		using const_iterator = dynarray< T >::const_iterator;

		dynmat() : data_(), cols_() {}
		dynmat( size_t cols, size_t rows, const T& value = T() ) : data_( rows * cols, value ), cols_( cols ) {}

		const T& operator()( index_t col, index_t row ) const { return data_[ row * cols_ + col ]; }
		T& operator()( index_t col, index_t row ) { return data_[ row * cols_ + col ]; }

		void resize( size_t cols, size_t rows ) { data_.resize( cols * rows ); cols_ = cols_; }

		size_t cols() const { return cols_; }
		size_t rows() const { return data_.size() / cols; }

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
