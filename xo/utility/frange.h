#pragma once

#include <cmath>
#include <iterator>

namespace xo
{
	// floating point range with even steps and no accumulated error
	// #perf: see if we can store less than 4 floats in each iterator
	template< typename T >
	struct frange
	{
		using value_type = T;
		frange( T b, T e, T s ) : begin_( b ), end_( e ), steps_( std::round( ( e - b ) / s ) ) {}
		frange( const frange< T >& r, T s = T( 1 ) ) : begin_( r.begin_ ), end_( r.end_ ), steps_( s ) {}

		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = T;
			using pointer = T*;
			using reference = T&;

			iterator( T s, T b, T e, T steps ) : s_( s ), b_( b ), e_( e ), steps_( steps ) {}
			T s_, b_, e_, steps_;
			T operator++() { return s_ += T( 1 ); }
			T operator++( int ) { s_ += T( 1 ); return steps_ - T( 1 ); }
			bool operator==( const iterator& other ) { return other.s_ == s_; }
			bool operator!=( const iterator& other ) { return other.s_ != s_; }
			T operator*() { return b_ + s_ * ( e_ - b_ ) / steps_; }
		};

		iterator begin() const { return iterator( T( 0 ), begin_, end_, steps_ ); }
		iterator end() const { return iterator( steps_ + 1, begin_, end_, steps_ ); }

		T begin_, end_, steps_;
	};
}
