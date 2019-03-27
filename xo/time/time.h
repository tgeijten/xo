#pragma once

#include "xo/string/string_type.h"
#include "xo/string/string_cast.h"

namespace xo
{
	struct time
	{
		using storage_t = long long;

		constexpr time() : internal_( 0 ) {}
		constexpr time( unsigned long long nanoseconds ) : internal_( storage_t( nanoseconds ) ) {}

		double seconds() const { return double( internal_ ) / 1e9; }
		storage_t milliseconds() const { return internal_ / 1'000'000; }
		storage_t microseconds() const { return internal_ / 1'000; }
		storage_t nanoseconds() const { return internal_; }

		bool operator<( time o ) const { return internal_ < o.internal_; }
		bool operator>( time o ) const { return internal_ > o.internal_; }
		bool operator==( time o ) const { return internal_ == o.internal_; }
		bool operator!=( time o ) const { return internal_ != o.internal_; }

		time operator-( time o ) { return internal_ - o.internal_; }
		time operator+( time o ) { return internal_ + o.internal_; }
		time& operator-=( time o ) { internal_ -= o.internal_; return *this; }
		time& operator+=( time o ) { internal_ += o.internal_; return *this; }

	private:
		storage_t internal_;
	};

	string to_str( time v ) { return to_str( v.seconds() ); }

	inline namespace literals {
		constexpr time operator ""_ns( unsigned long long ns ) { return time{ ns }; }
		constexpr time operator ""_ms( unsigned long long ms ) { return time( ms * 1'000'000 ); }
		constexpr time operator ""_s( unsigned long long s ) { return time( s * 1'000'000'000 ); }
		constexpr time operator ""_m( unsigned long long m ) { return time( m * 60 * 1'000'000'000 ); }
		constexpr time operator ""_h( unsigned long long h ) { return time( h * 60 * 60 * 1'000'000'000 ); }
	}
}
