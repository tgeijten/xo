#pragma once

#include "xo/string/string_type.h"
#include "xo/string/string_cast.h"

namespace xo
{
	struct XO_API time
	{
		using storage_t = long long;

		constexpr time() : internal_( 0 ) {}
		constexpr time( storage_t nanoseconds ) : internal_( nanoseconds ) {}

		double seconds() const { return double( internal_ ) / 1e9; }
		double millisecondsd() const { return double( internal_ ) / 1e6; }
		storage_t milliseconds() const { return internal_ / 1'000'000; }
		storage_t microseconds() const { return internal_ / 1'000; }
		storage_t nanoseconds() const { return internal_; }

		bool operator<( time o ) const { return internal_ < o.internal_; }
		bool operator>( time o ) const { return internal_ > o.internal_; }
		bool operator==( time o ) const { return internal_ == o.internal_; }
		bool operator!=( time o ) const { return internal_ != o.internal_; }

		time operator-( time o ) const { return internal_ - o.internal_; }
		time operator+( time o ) const { return internal_ + o.internal_; }
		time& operator-=( time o ) { internal_ -= o.internal_; return *this; }
		time& operator+=( time o ) { internal_ += o.internal_; return *this; }

		template< typename T > time operator/( T v ) const { return time( storage_t( internal_ / v ) ); }
		template< typename T > time operator*( T v ) const { return time( storage_t( internal_ * v ) ); }

	private:
		storage_t internal_;
	};

	inline string to_str( time v ) { return to_str( v.seconds() ); }


	inline namespace literals {
		constexpr time operator ""_ns( unsigned long long ns ) { return time( time::storage_t( ns ) ); }
		constexpr time operator ""_ms( unsigned long long ms ) { return time( time::storage_t( ms * 1'000'000 ) ); }
		constexpr time operator ""_s( unsigned long long s ) { return time( time::storage_t( s * 1'000'000'000 ) ); }
		constexpr time operator ""_m( unsigned long long m ) { return time( time::storage_t( m * 60 * 1'000'000'000 ) ); }
		constexpr time operator ""_h( unsigned long long h ) { return time( time::storage_t( h * 60 * 60 * 1'000'000'000 ) ); }
	}
}
