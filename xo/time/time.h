#pragma once

namespace xo
{
	struct time
	{
		constexpr time( unsigned long long nanoseconds = 0 ) : internal_( nanoseconds ) {}
		double seconds() const { return double( internal_ ) / 1e9; }
		long long milliseconds() const { return internal_ / 1'000'000; }
		long long microseconds() const { return internal_ / 1'000; }
		long long nanoseconds() const { return internal_; }

		bool operator<( time o ) const { return internal_ < o.internal_; }
		bool operator>( time o ) const { return internal_ > o.internal_; }
		bool operator==( time o ) const { return internal_ == o.internal_; }
		bool operator!=( time o ) const { return internal_ != o.internal_; }

		time operator-( time o ) { return internal_ - o.internal_; }
		time operator+( time o ) { return internal_ + o.internal_; }
		time& operator-=( time o ) { internal_ -= o.internal_; return *this; }
		time& operator+=( time o ) { internal_ += o.internal_; return *this; }

	private:
		unsigned long long internal_;
	};

	inline namespace literals {
		constexpr time operator ""_ns( unsigned long long ns ) { return time{ ns }; }
		constexpr time operator ""_ms( unsigned long long ms ) { return time( ms * 1'000'000 ); }
		constexpr time operator ""_s( unsigned long long s ) { return time( s * 1'000'000'000 ); }
		constexpr time operator ""_m( unsigned long long m ) { return time( m * 60 * 1'000'000'000 ); }
		constexpr time operator ""_h( unsigned long long h ) { return time( h * 60 * 60 * 1'000'000'000 ); }
	}
}
