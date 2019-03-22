#pragma once

namespace xo
{
	struct time
	{
		time( long long nanoseconds ) : ns( nanoseconds ) {}
		double seconds() const { return double( ns ) / 1e9; }
		long long milliseconds() const { return ns / 1000000; }
		long long microseconds() const { return ns / 1000; }
		long long nanoseconds() const { return ns; }

		bool operator<( time o ) const { return ns < o.ns; }
		bool operator>( time o ) const { return ns > o.ns; }
		bool operator==( time o ) const { return ns == o.ns; }
		bool operator!=( time o ) const { return ns != o.ns; }

	private:
		long long ns;
	};

	inline namespace literals {
		constexpr time operator ""_ns( long long ns ) { return time( ns ); }
		constexpr time operator ""_ms( long long ms ) { return time( ms * 1000000 ); }
		constexpr time operator ""_s( long long s ) { return time( s * 1000000000 ); }
		constexpr time operator ""_m( long long m ) { return time( h * 60 * 1000000000 ); }
		constexpr time operator ""_h( long long h ) { return time( h * 60 * 60 * 1000000000 ); }
	}
}
