#pragma once

#include <cstddef>

namespace xo
{
	struct time
	{
		using storage_t = long long;

		constexpr time() : internal_( 0 ) {}
		explicit constexpr time( storage_t nanoseconds ) : internal_( nanoseconds ) {}

		template< typename T > T seconds() const { return static_cast<T>( 1e-9 * internal_ ); }
		double secondsd() const { return seconds<double>(); }
		float secondsf() const { return seconds<float>(); }
		template< typename T > T milliseconds() const { return static_cast<T>( 1e-6 * internal_ ); }
		double milliseconds() const { return milliseconds<double>(); }
		float millisecondsf() const { return milliseconds<float>(); }
		storage_t nanoseconds() const { return internal_; }
		double nanosecondsd() const { return double( internal_ ); }

		bool operator<( time o ) const { return internal_ < o.internal_; }
		bool operator<=( time o ) const { return internal_ <= o.internal_; }
		bool operator>( time o ) const { return internal_ > o.internal_; }
		bool operator>=( time o ) const { return internal_ >= o.internal_; }
		bool operator==( time o ) const { return internal_ == o.internal_; }
		bool operator!=( time o ) const { return internal_ != o.internal_; }

		time operator-( time o ) const { return time( internal_ - o.internal_ ); }
		time operator+( time o ) const { return time( internal_ + o.internal_ ); }
		time operator/( size_t n ) const { return time( internal_ / n ); }
		time& operator-=( time o ) { internal_ -= o.internal_; return *this; }
		time& operator+=( time o ) { internal_ += o.internal_; return *this; }
		time& operator/=( size_t n ) { internal_ /= n; return *this; }

		// ratio between times
		double operator/( time v ) const { return double( internal_ ) / double( v.internal_ ); }
		double operator*( time v ) const { return double( internal_ ) * double( v.internal_ ); }

		bool is_zero() const { return internal_ == 0; }

		template< typename T > time operator/( T v ) const { return time( storage_t( internal_ / v ) ); }
		template< typename T > time operator*( T v ) const { return time( storage_t( internal_ * v ) ); }

	private:
		storage_t internal_;
	};

	template< typename T > time time_from_seconds( T s ) { return time( time::storage_t( double( s ) * 1e9 + 0.5 ) ); }
	template< typename T > time time_from_milliseconds( T s ) { return time( time::storage_t( double( s ) * 1e6 + 0.5 ) ); }
	template< typename T > time time_from_nanoseconds( T s ) { return time( time::storage_t( double( s ) + 0.5 ) ); }

	inline namespace literals {
		constexpr time operator ""_ns( unsigned long long ns ) { return time( time::storage_t( ns ) ); }
		constexpr time operator ""_ms( unsigned long long ms ) { return time( time::storage_t( ms * 1'000'000 ) ); }
		constexpr time operator ""_s( unsigned long long s ) { return time( time::storage_t( s * 1'000'000'000 ) ); }
		constexpr time operator ""_minute( unsigned long long m ) { return time( time::storage_t( m * 60 * 1'000'000'000 ) ); }
		constexpr time operator ""_hour( unsigned long long h ) { return time( time::storage_t( h * 60 * 60 * 1'000'000'000 ) ); }
		constexpr time operator ""_day( unsigned long long d ) { return time( time::storage_t( d * 24 * 60 * 60 * 1'000'000'000 ) ); }
		constexpr time operator ""_year( unsigned long long y ) { return time( time::storage_t( y * 365 * 24 * 60 * 60 * 1'000'000'000 ) ); }
	}
}
