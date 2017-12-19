#pragma once

#include "xo/utility/types.h"

namespace xo
{
	template< typename T > struct vec_
	{
		using iterator = typename T*;
		using const_iterator = typename const T*;

		vec_( size_t n ) : data_( n ) {}
		vec_( size_t n, const T& v ) : data_( n, v ) {}
		vec_( const vec_<T>& o ) : data_( o.data_ ) {}
		vec_( vec_<T>&& o ) : data_( std::move( o.data_ ) ) {}

		/// assignment
		vec_<T>& operator=( const vec_<T>& o ) { data_ = o.data_; }
		vec_<T>& operator=( vec_<T>&& o ) { data_ = std::move( o.data_ ); }

		/// element access
		const T& operator[]( index_t idx ) const { return data_[ idx ]; }
		T& operator[]( index_t idx ) { return data_[ idx ]; }

		size_t size() const { return end() - begin(); }

		iterator begin() { return data_.begin(); }
		iterator end() { return data_.end(); }
		const_iterator begin() const { return data_.begin(); }
		const_iterator end() const { return data_.end(); }

		/// const properties
		T length() const { return sqrt( squared_length() ); }
		T squared_length() const { T sum = T(); for ( auto& e : data_ ) sum += e * e; return sum; }
		bool is_null() const { return for ( auto& e : data_ ) if ( e != T( 0 ) ) return false; return true; }

	private:
		dynarray< T > data_;
	};

	/// template instantiations
	typedef vec_< float > vecf;
	typedef vec_< double > vecd;
}
