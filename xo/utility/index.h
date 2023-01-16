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
		constexpr index_class() : idx( invalid_index_value() ) {}
		index_class( const index_class& o ) = default;
		index_class( index_class&& o ) noexcept : idx( o.idx ) { o.reset(); }

		// explicit conversion constructors
		explicit constexpr index_class( index_type v ) : idx( v ) {}
		template<typename U> explicit constexpr index_class( index_class<U, I> o ) : idx( o.value() ) {}
		explicit constexpr index_class( size_t i ) : idx( i <= max_value() ? index_type( i ) : invalid_index_value() ) {}
		explicit constexpr index_class( int i ) : idx( i >= 0 && unsigned( i ) <= max_value() ? index_type( i ) : invalid_index_value() ) {}

		// default copy and move assignment
		index_class& operator=( const index_class& o ) { idx = o.idx; return *this; }
		index_class& operator=( index_class&& o ) noexcept { idx = o.idx; o.reset(); return *this; }

		index_type value() const { return idx; }
		index_type max_value() const { return invalid_index_value() - 1; }
		explicit operator bool() const { return idx != invalid_index_value(); }

		void reset() { idx = invalid_index_value(); }

		template< typename C > auto iter( const C& c ) const { return c.begin() + idx; }
		template< typename C > auto iter( C& c ) const { return c.begin() + idx; }

		template< typename C > static index_class end( const C& c ) { return index_class( index_type( c.size() ) ); }
		template< typename C > static index_class back( const C& c ) { return index_class( index_type( c.size() ) - 1 ); }

		index_class operator+( index_type ofs ) const { return index_class( idx + ofs ); }
		index_class operator-( index_type ofs ) const { return index_class( idx - ofs ); }

		index_class& operator++() { ++idx; return *this; }
		index_class operator++( int ) { index_class org( idx ); ++idx; return org; }

		friend bool operator<( const index_class a, const index_class b ) { return a.idx < b.idx; }
		friend bool operator==( const index_class a, const index_class b ) { return a.idx == b.idx; }
		friend bool operator!=( const index_class a, const index_class b ) { return a.idx != b.idx; }

		index_type idx;

	protected:
		static constexpr index_type invalid_index_value() { return ~index_type( 0 ); }
	};
}
