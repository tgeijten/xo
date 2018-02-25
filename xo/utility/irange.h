#pragma once

#include <xutility>

namespace xo
{
	template< typename T >
	struct irange
	{
		typedef T value_type;
		irange() : begin_( T( 0 ) ), end_( T( 0 ) ) {}
		irange( T b, T e ) : begin_( b ), end_( e ) {}

		struct iterator : public std::iterator< std::forward_iterator_tag, T, T >
		{
			typedef T value_type;
			iterator( T v ) : value_( v ) {}
			T value_;
			T operator++() { return ++value_; }
			T operator++( int ) { return value++; }
			iterator operator+( const T& offset ) { return iterator( value_ + offset ); }

			bool operator==( const iterator& other ) { return other.value_ == value_; }
			bool operator!=( const iterator& other ) { return other.value_ != value_; }
			T operator-( const iterator& other ) { return value_ - other.value_; }
			T operator*() { return value_; }
		};

		iterator begin() const { return iterator( begin_ ); }
		iterator end() const { return iterator( end_ ); }

		T begin_, end_;
	};

	template< typename T >
	struct irange_step
	{
		typedef T value_type;
		irange_step( T b, T e, T s = T( 1 ) ) : begin_( b ), end_( e ), step_( s ) {}
		irange_step( const irange< T >& r, T s = T( 1 ) ) : begin_( r.begin_ ), end_( r.end_ ), step_( s ) {}

		struct iterator : public std::iterator< std::forward_iterator_tag, T, T >
		{
			typedef T value_type;
			iterator( T v, T s ) : value_( v ), step_( s ) {}
			T value_, step_;
			T operator-( const iterator& other ) { return ( value_ - other.value_ ) / step_; }
			T operator++() { return value_ += step_; }
			T operator++( int ) { value_ += step_; return value_ - step_; }
			bool operator==( const iterator& other ) { return other.value_ == value_; }
			bool operator!=( const iterator& other ) { return other.value_ != value_; }
			T operator*() { return value_; }
		};

		iterator begin() const { return iterator( begin_, step_ ); }
		iterator end() const { return iterator( end_, step_ ); }

		T begin_, end_, step_;
	};

	// TODO: SFINAE to make sure it's integer
	template< typename T > irange< T > make_irange( T b, T e ) { return irange< T >( b, e ); }
	template< typename T > irange< T > make_irange( T e ) { return irange< T >( T( 0 ), e ); }
	template< typename T > irange_step< T > make_irange( T b, T e, T s ) { return irange_step< T >( b, e, s ); }
	template< typename T > irange_step< T > make_irange( const irange< T >& r, T s ) { return irange_step< T >( r, s ); }
}
