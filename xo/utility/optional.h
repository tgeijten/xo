#pragma once

#include <limits>
#include <type_traits>
#include <iosfwd>

namespace xo
{
	template< typename T, typename E = void > struct optional
	{
		using value_type = T;
		optional() : has_value_( false ) {}
		optional( const T& v ) : value_( v ), has_value_( true ) {}
		optional( T&& v ) : value_( std::move( v ) ), has_value_( true ) {}
		optional< T, E >& operator=( const T& v ) { value_ = v; has_value_ = true; return *this; }
		optional< T, E >& operator=( T&& v ) { value_ = std::move( v ); has_value_ = true; return *this; }
		operator bool() const { return has_value_; }
		const T& operator*() const { return value_; }
		void reset() { has_value_ = false; }
		T value_;
		bool has_value_;
	};

	template< typename T > struct optional< T, typename std::enable_if< std::is_integral<T>::value >::type >
	{
		using value_type = T;
		optional() : value_( sentinel<T>() ) {}
		optional( const T& v ) : value_( v ) {}
		optional< T >& operator=( const T& v ) { value_ = v; return *this; }
		operator bool() const { return value_ != sentinel<T>(); }
		const T& operator*() const { return value_; }
		void reset() { value_ = sentinel<T>(); }
		T value_;
	};

	template< typename T > struct optional< T, typename std::enable_if< std::is_floating_point<T>::value >::type >
	{
		using value_type = T;
		optional() : value_( std::numeric_limits<T>::quiet_NaN() ) {}
		optional( const T& v ) : value_( v ) {}
		optional< T >& operator=( const T& v ) { value_ = v; return *this; }
		operator bool() const { return value_ == value_; }
		const T& operator*() const { return value_; }
		void reset() { value_ = std::numeric_limits<T>::quiet_NaN(); }
		T value_;
	};

	template< typename T > std::istream& operator>>( std::istream& str, optional< T >& v )
	{ T tmp; str >> tmp; if ( !str.fail() ) v = tmp; return str; }

	using optional_int = optional< int >;
	using optional_size = optional< size_t >;
	using optional_float = optional< float >;
	using optional_double = optional< double >;
}
