#pragma once

#include "xo/system/xo_config.h"
#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include "xo/geometry/angle.h"

namespace xo
{
	XO_API string to_str( float value );
	XO_API bool from_str( const string& s, float& v );

	XO_API string to_str( double value );
	XO_API bool from_str( const string& s, double& v );

	XO_API string to_str( bool value );
	XO_API bool from_str( const string& s, bool& v );

	XO_API string to_str( int value );
	XO_API bool from_str( const string& s, int& v );

	XO_API string to_str( unsigned int value );
	XO_API bool from_str( const string& s, unsigned int& v );

	XO_API string to_str( long value );
	XO_API bool from_str( const string& s, long& v );

	XO_API string to_str( long long value );
	XO_API bool from_str( const string& s, long long& v );

	XO_API string to_str( unsigned long value );
	XO_API bool from_str( const string& s, unsigned long& v );

	XO_API string to_str( unsigned long long value );
	XO_API bool from_str( const string& s, unsigned long long& v );

	inline string to_str( const char* value ) { return string( value ); }
	inline bool from_str( const string& s, const char*& v ) { v = s.c_str(); return true; }

	inline string to_str( string value ) { return value; }
	inline bool from_str( const string& s, string& v ) { v = s; return true; }

	XO_API bool from_str( const string& s, version& ver );
	XO_API string to_str( const version& v );

	XO_API bool from_str( const string& s, time& v );
	XO_API string to_str( const time& v );

	XO_API bool from_str( const string& s, path& v );
	XO_API string to_str( const path& p );
}
