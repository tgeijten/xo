#pragma once

#include "xo/xo_types.h"
#include "xo/numerical/constants.h"

#include <type_traits> // #todo: consider version without this header (using specialization)

namespace xo
{
	template< typename T, typename E = void > struct optional;

	/// optional value for integrals (not bool!) that uses a sentinel to represent no value
	template< typename T > struct optional< T, typename std::enable_if_t< std::is_integral_v<T> && !std::is_same_v<T, bool> > >
	{
		using value_type = T;
		optional() : value_( constants<T>::sentinel() ) {}
		optional( const T& v ) : value_( v ) {}
		optional& operator=( const T& v ) { value_ = v; return *this; }
		explicit operator bool() const { return value_ != constants<T>::sentinel(); }
		const T& operator*() const { return value_; }
		T& operator*() { return value_; }
		const T* operator->() const { return &value_; }
		T* operator->() { return &value_; }
		void reset() { value_ = constants<T>::sentinel(); }
		const T& value() const { xo_error_if( !*this, "xo::optional has no value" ); return value_; }
		T value_or( const T& default_value ) const { return bool( *this ) ? value_ : default_value; }
	private:
		T value_;
	};

	/// optional value for floating point numbers that uses NaN to represent no value
	template< typename T > struct optional< T, typename std::enable_if_t< std::is_floating_point_v<T> > >
	{
		using value_type = T;
		optional() : value_( std::numeric_limits<T>::quiet_NaN() ) {}
		optional( const T& v ) : value_( v ) {}
		optional& operator=( const T& v ) { value_ = v; return *this; }
		explicit operator bool() const { return value_ == value_; }
		const T& operator*() const { return value_; }
		T& operator*() { return value_; }
		const T* operator->() const { return &value_; }
		T* operator->() { return &value_; }
		void reset() { value_ = std::numeric_limits<T>::quiet_NaN(); }
		const T& value() const { xo_error_if( !*this, "xo::optional has no value" ); return value_; }
		T value_or( const T& default_value ) const { return bool( *this ) ? value_ : default_value; }
	private:
		T value_;
	};

	/// optional value that works with any value type
	template< typename T > struct optional< T, typename std::enable_if_t< !std::is_floating_point_v<T> && !( std::is_integral_v<T> && !std::is_same_v<T, bool> ) > >
	{
		using value_type = T;
		optional() : value_(), has_value_( false ) {}
		optional( const T& v ) : value_( v ), has_value_( true ) {}
		optional( T&& v ) : value_( std::move( v ) ), has_value_( true ) {}
		optional& operator=( const T& v ) { value_ = v; has_value_ = true; return *this; }
		optional& operator=( T&& v ) { value_ = std::move( v ); has_value_ = true; return *this; }
		explicit operator bool() const { return has_value_; }
		const T& operator*() const { return value_; }
		T& operator*() { return value_; }
		const T* operator->() const { return &value_; }
		T* operator->() { return &value_; }
		void reset() { value_ = T(); has_value_ = false; }
		const T& value() const { xo_error_if( !*this, "xo::optional has no value" ); return value_; }
		T value_or( const T& default_value ) const { return bool( *this ) ? value_ : default_value; }
	private:
		T value_;
		bool has_value_;
	};
}
