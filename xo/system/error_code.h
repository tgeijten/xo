#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"

namespace xo
{
	struct error_code
	{
		error_code( int errcode = 0, string msg = string( "" ) ) : error_( errcode ), message_( msg ) {}
		error_code& set( int errcode, string msg = string( "" ) ) { error_ = errcode; message_ = msg; return *this; }
		bool bad() const { return error_ != 0; }
		bool good() const { return error_ == 0; }
		int code() const { return error_; }
		const string& message() const { return message_; }

	private:
		int error_;
		string message_;
	};

	XO_API bool try_set_error( error_code* ec, string msg );
	XO_API void set_error_or_throw( error_code* ec, string msg );
}
