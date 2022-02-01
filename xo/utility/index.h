#pragma once

#include "xo/xo_types.h"

namespace xo
{
	/// strongly typed index
	template< typename T, typename I = uint32 >
	struct index_class
	{
		using class_type = T;
		using index_type = I;

		// default, copy and move constructor
		constexpr index_class() : value_( invalid() ) {}
		index_class( const index_class& o ) = default;
		index_class( index_class&& o ) noexcept : value_( o.value_ ) { o.reset(); }

		// explicit conversion constructors
		explicit constexpr index_class( index_type v ) : value_( v ) {}
		template<typename U> explicit constexpr index_class( index_class<U, I> o ) : value_( o.value() ) {}

		// default copy and move assignment
		index_class& operator=( const index_class& o ) { value_ = o.value_; return *this; }
		index_class& operator=( index_class&& o ) noexcept { std::swap( value_, o.value_ ); return *this; }

		explicit operator bool() const { return value_ != invalid(); }

		const index_type& value() const { return value_; }
		index_type& value() { return value_; }

		void reset() { value_ = invalid(); }

		template< typename C > auto iter( const C& c ) const { return c.begin() + value_; }
		template< typename C > auto iter( C& c ) const { return c.begin() + value_; }

		index_class operator+( index_type ofs ) const { return index_class( value_ + ofs ); }
		index_class operator-( index_type ofs ) const { return index_class( value_ - ofs ); }

		friend bool operator<( const index_class a, const index_class b ) { return a.value_ < b.value_; }
		friend bool operator==( const index_class a, const index_class b ) { return a.value_ == b.value_; }
		friend bool operator!=( const index_class a, const index_class b ) { return a.value_ != b.value_; }

	protected:
		static constexpr index_type invalid() { return ~index_type( 0 ); }
		index_type value_;
	};
}
