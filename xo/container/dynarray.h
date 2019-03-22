#pragma once

#include "xo/utility/types.h"
#include "xo/utility/pointer_iterator.h"
#include "xo/utility/pointer_types.h"

namespace xo
{
	/// dynamically sized array
	template< typename T >
	class dynarray
	{
	public:
		using iterator = pointer_iterator< T >;
		using const_iterator = pointer_iterator< const T >;

		dynarray() : data_( nullptr ), end_( nullptr ) {}
		dynarray( size_t n, const T& v = T() ) : data_( new T[ n ] ), end_( data_.get() + n ) { assign( v ); }
		dynarray( const dynarray& o ) : data_( new T[ o.size() ] ), end_( data_.get() + o.size() ) { xo::copy( o.begin(), o.end(), begin() ); }
		dynarray( dynarray&& o ) : data_( std::move( o.data_ ) ), end_( o.end_ ) {}
		dynarray<T>& operator=( const dynarray& o ) { data_ = u_ptr< T[] >( new T[ o.size() ] ); end_ = data_.get() + o.size(); xo::copy( o.begin(), o.end(), begin() ); return *this; }
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

		void resize( size_t n, const T& v = T() ) { data_.reset( new T[ n ] ); end_ = data_.get() + n; assign( v ); }

		T* data() { return data_.get(); }
		const T* data() const { return data_.get(); }

	private:
		u_ptr< T[] > data_;
		T* end_;
	};

	template< typename T >
	bool operator==( const dynarray< T >& v1, const dynarray< T >& v2 ) {
		if ( v1.size() == v2.size() ) {
			for ( index_t i = 0; i < v1.size(); ++i )
				if ( v1[ i ] != v2[ i ] )
					return false;
			return true;
		}
		else return false;
	}

	template< typename T >
	bool operator!=( const dynarray< T >& v1, const dynarray< T >& v2 )
	{ return !( v1 == v2 ); }
}
