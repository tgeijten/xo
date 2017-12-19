#pragma once

#include <algorithm>
#include "xo/utility/types.h"

namespace xo
{
	template< typename T >
	class dynarray
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		dynarray() : data_( nullptr ), end_( nullptr ) {}
		dynarray( size_t n ) : data_( new T[ n ] ), end_( data_.get() + n ) {}
		dynarray( size_t n, const T& v ) : data_( new T[ n ] ), end_( data_.get() + n ) { assign( v ); }
		dynarray( const dynarray& o ) : data_( new T[ o.size() ] ), end_( data_.get() + o.size() ) { std::copy( o.begin(), o.end(), begin() ); }
		dynarray( dynarray&& o ) : data_( std::move( o.data_ ) ), end_( o.end_ ) {}
		dynarray<T>& operator=( const dynarray& o ) { data_ = u_ptr< T[] >( new T[ o.size() ] ); end_ = data_.get() + o.size(); std::copy( o.begin(), o.end(), begin() ); return *this; }
		dynarray<T>& operator=( dynarray&& o ) { data_ = std::move( o.data_ ); end_ = o.end_; return *this; }
		~dynarray() {}

		T& operator[]( size_t i ) { return data_[ i ]; }
		const T& operator[]( size_t i ) const { return data_[ i ]; }

		T& at( size_t i ) { xo_error_if( i >= size(), "dynarray index out of bounds" ); return data_[ i ]; }
		const T& at( size_t i ) const { xo_error_if( i >= size(), "dynarray index out of bounds" ); return data_[ i ]; }

		iterator begin() { return data_.get(); }
		const_iterator begin() const { return data_.get(); }
		const_iterator cbegin() const { return data_.get(); }

		iterator end() { return end_; }
		const_iterator end() const { return end_; }
		const_iterator cend() const { return end_; }

		size_t size() const { return end() - begin(); }
		void assign( const T& v ) { std::fill( begin(), end(), v ); }

		T* data() { return data_.get(); }
		const T* data() const { return data_.get(); }

	private:
		u_ptr< T[] > data_;
		T* end_;
	};
}
