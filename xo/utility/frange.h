#pragma once

#include <cmath>
#include <iterator>
#include "xo/numerical/bounds.h"

namespace xo
{
	// floating point range with even steps and no accumulated error
	// #perf: see if we can store less than 4 floats in each iterator
	template< typename T >
	struct frange
	{
		using value_type = T;
		frange( T lower, T upper, T increment ) : min_( lower ), max_( upper ), max_step_( std::round( ( upper - lower ) / increment ) ) {}
		frange( T lower, T upper, int steps ) : min_( lower ), max_( upper ), max_step_( steps - 1 ) {}
		frange( bounds<T> bounds, T increment ) : min_( bounds.lower ), max_( bounds.upper ), max_step_( std::round( bounds.range() / increment ) ) {}
		frange( bounds<T> bounds, int steps ) : min_( bounds.lower ), max_( bounds.upper ), max_step_( steps - 1 ) {}
		
		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = size_t;
			using pointer = T*;
			using reference = T&;

			iterator( T step, T b, T e, T steps ) : step_( step ), min_( b ), max_( e ), max_step_( steps ) {}
			T step_, min_, max_, max_step_;
			T operator++() { return step_ += T( 1 ); }
			T operator++( int ) { step_ += T( 1 ); return max_step_ - T( 1 ); }
			bool operator==( const iterator& other ) { return other.step_ == step_; }
			bool operator!=( const iterator& other ) { return other.step_ != step_; }
			T operator*() { return min_ + step_ * ( max_ - min_ ) / max_step_; }
		};

		iterator begin() const { return iterator( T( 0 ), min_, max_, max_step_ ); }
		iterator end() const { return iterator( max_step_ + 1, min_, max_, max_step_ ); }
		size_t size() const { return size_t( max_step_ ); }

		T min_, max_, max_step_;
	};
}
