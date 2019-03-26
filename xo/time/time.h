#pragma once

namespace xo
{
	struct time
	{
		constexpr time( unsigned long long nanoseconds ) : ns( nanoseconds ) {}
		double seconds() const { return double( ns ) / 1e9; }
		long long milliseconds() const { return ns / 1'000'000; }
		long long microseconds() const { return ns / 1'000; }
		long long nanoseconds() const { return ns; }

		bool operator<( time o ) const { return ns < o.ns; }
		bool operator>( time o ) const { return ns > o.ns; }
		bool operator==( time o ) const { return ns == o.ns; }
		bool operator!=( time o ) const { return ns != o.ns; }

	private:
		unsigned long long ns;
	};

	inline namespace literals {
		constexpr time operator ""_ns( unsigned long long ns ) { return time{ ns }; }
		constexpr time operator ""_ms( unsigned long long ms ) { return time( ms * 1'000'000 ); }
		constexpr time operator ""_s( unsigned long long s ) { return time( s * 1'000'000'000 ); }
		constexpr time operator ""_m( unsigned long long m ) { return time( m * 60 * 1'000'000'000 ); }
		constexpr time operator ""_h( unsigned long long h ) { return time( h * 60 * 60 * 1'000'000'000 ); }
	}
}
