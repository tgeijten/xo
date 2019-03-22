#pragma once

#include "xo/system/xo_api.h"
#include <iterator>

namespace xo
{
	template< typename T >
	struct pointer_iterator
	{
#ifdef XO_COMP_MSVC
		typedef pointer_iterator _Unchecked_type; // prevent warning C4996
#endif
		using value_type = T;
		using difference_type = long;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

		pointer_iterator( T* v ) : value_( v ) {}
		pointer_iterator( const pointer_iterator& o ) : value_( o.value_ ) {}
		pointer_iterator& operator=( const pointer_iterator& o ) { value_ = o.value_; return *this; }
		pointer_iterator& operator++() { ++value_; return *this; }
		pointer_iterator operator++( int ) { pointer_iterator tmp( value_ ); ++value_; return tmp; }
		pointer_iterator& operator--() { --value_; return *this; }
		pointer_iterator operator--( int ) { pointer_iterator tmp( value_ ); --value_; return tmp; }
		pointer_iterator operator+( int ofs ) { return pointer_iterator( value_ + ofs ); }
		difference_type operator-( const pointer_iterator& other ) { return difference_type( value_ - other.value_ ); }
		bool operator==( const pointer_iterator& other ) const { return value_ == other.value_; }
		bool operator!=( const pointer_iterator& other ) const { return value_ != other.value_; }
		bool operator<( const pointer_iterator& other ) const { return value_ < other.value_; }
		T& operator*() { return *value_; }

		T* value_;
	};
}
