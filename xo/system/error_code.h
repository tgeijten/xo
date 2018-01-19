#pragma once

#include "xo/string/string_type.h"
#include "assert.h"

namespace xo
{
	struct error_code
	{
		error_code( int errcode = 0, string msg = string( "" ) ) : error_( errcode ), message_( msg ) {}
		error_code& set( int errcode, string msg = string( "" ) ) { error_ = errcode; message_ = msg; return *this; }
		bool error() { return error_ != 0; }
		bool ok() { return error_ == 0; }
		int code() { return error_; }
		const string& message() { return message_; }

	private:
		int error_;
		string message_;
	};

	inline error_code no_error() { return error_code(); }
	inline error_code error( string msg ) { return error_code( -1, msg ); }
	inline bool try_set_error( error_code* ec, string msg ) { if ( ec ) ec->set( -1, msg ); return ec != nullptr; }
	inline void set_error_or_throw( error_code* ec, string msg ) { if ( ec ) ec->set( -1, msg ); else xo_error( msg ); }
}
