#pragma once

#include "xo/string/string_type.h"

namespace xo
{
	struct error_message {
		error_message() : message_() {}
		error_message( const error_message& ) = default;
		error_message( const char* msg ) : message_( msg ) {};
		error_message( const string& msg ) : message_( msg ) {};
		error_message( error_message&& ) noexcept = default;
		error_message( string&& msg ) noexcept : message_( std::move( msg ) ) {};

		error_message& operator=( const error_message& ) = default;
		error_message& operator=( const char* msg ) { message_ = msg; }
		error_message& operator=( const string& msg ) { message_ = msg; };
		error_message& operator=( error_message&& ) noexcept = default;
		error_message& operator=( string&& msg ) noexcept { message_ = std::move( msg ); }

		bool good() const { return message_.empty(); }
		bool bad() const { return !message_.empty(); }
		const string& message() const { return message_; }

	private:
		string message_;
	};

	template< typename T, typename E = error_message >
	struct result {
		result() : value_(), error_() {}
		result( const result& ) = default;
		result( const T& v ) : value_( v ), error_() {}
		result( const E& e ) : value_(), error_( e ) {}
		result( result&& ) noexcept = default;
		result( T&& v ) : value_( std::move( v ) ), error_() {}
		result( E&& e ) : value_(), error_( std::move( e ) ) {}

		result& operator=( const result& ) = default;
		result& operator=( const T& v ) { value_ = v; error_ = E(); return *this; }
		result& operator=( const E& e ) { error_ = e; return *this; }
		result& operator=( result&& ) noexcept = default;
		result& operator=( T&& v ) noexcept { value_ = std::move( v ); error_ = E(); return *this; }
		result& operator=( E&& e ) noexcept { error_ = std::move( e ); return *this; }

		explicit operator bool() const { return error_.good(); }
		const T& value() const { return value_; }
		const E& error() const { return error_; }

	private:
		T value_;
		E error_;
	};
}
