#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"

namespace xo
{
	/// convert data from string
	XO_API bool from_str( const string& s, float& v );
	XO_API bool from_str( const string& s, double& v );
	XO_API bool from_str( const string& s, bool& v );
	XO_API bool from_str( const string& s, int v );
	XO_API bool from_str( const string& s, unsigned int& v );
	XO_API bool from_str( const string& s, long& v );
	XO_API bool from_str( const string& s, long long& v );
	XO_API bool from_str( const string& s, unsigned long& v );
	XO_API bool from_str( const string& s, unsigned long long& v );
	inline bool from_str( const string& s, const char*& v ) { v = s.c_str(); return true; }
	inline bool from_str( const string& s, string& v ) { v = s; return true; }

	/// convert data to string
	XO_API string to_str( float value );
	XO_API string to_str( double value );
	XO_API string to_str( bool value );
	XO_API string to_str( int value );
	XO_API string to_str( unsigned int value );
	XO_API string to_str( long value );
	XO_API string to_str( unsigned long value );
	XO_API string to_str( long long value );
	XO_API string to_str( unsigned long long value );
	inline string to_str( const char* value ) { return string( value ); }
	inline string to_str( string value ) { return value; }
}
