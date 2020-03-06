#pragma once

#include "xo/string/string_type.h"

namespace xo
{
	struct error_message {
		error_message() : message_() {}
		error_message( const char* msg ) : message_( msg ) {};
		error_message( const string& msg ) : message_( msg ) {};
		error_message( string&& msg ) : message_( std::move( msg ) ) {};

		explicit operator bool() const { return message_.empty(); }
		const string message() const { return message_; }

	private:
		string message_;
	};

	template< typename T, typename E = error_message >
	struct result {
		result() : value_(), error_() {}
		result( const T& v ) : value_( v ), error_() {}
		result( T&& v ) : value_( std::move( v ) ), error_() {}
		result( const E& e ) : value_(), error_( e ) {}
		result( E&& e ) : value_(), error_( std::move( e ) ) {}

		explicit operator bool() { return static_cast<bool>( error_ ); }
		const T& value() const { return value_; }
		const E& error() const { return error_; }

	public:
		T value_;
		E error_;
	};
}
