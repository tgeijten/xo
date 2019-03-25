#pragma once

#include "xo/system/xo_api.h"
#include "xo/system/assert.h"
#include "xo/string/string_type.h"

namespace xo
{
	/// convert data from string
	inline float from_str( const string& s, float default_value ) { char* p; auto v = std::strtof( s.c_str(), &p ); return p != s.c_str() ? v : default_value; }
	inline double from_str( const string& s, double default_value ) { char* p; auto v = std::strtod( s.c_str(), &p ); return p != s.c_str() ? v : default_value; }
	inline bool from_str( const string& s, bool default_value ) { return ( s == "1" || s == "true" ); }
	inline int from_str( const string& s, int default_value ) { char* p; auto v = (int)( std::strtol( s.c_str(), &p, 10 ) ); return p != s.c_str() ? v : default_value; }
	inline unsigned int from_str( const string& s, unsigned int default_value ) { char* p; auto v = (unsigned int)( std::strtol( s.c_str(), &p, 10 ) ); return p != s.c_str() ? v : default_value; }
	inline long from_str( const string& s, long default_value ) { char* p; auto v = std::strtol( s.c_str(), &p, 10 ); return p != s.c_str() ? v : default_value; }
	inline unsigned long from_str( const string& s, unsigned long default_value ) { char* p; auto v = std::strtoul( s.c_str(), &p, 10 ); return p != s.c_str() ? v : default_value; }
	inline size_t from_str( const string& s, size_t default_value ) { char* p; auto v = std::strtoul( s.c_str(), &p, 10 ); return p != s.c_str() ? v : default_value; }
	inline const char* from_str( const string& s, const char* default_value ) { return s.c_str(); }
	inline string from_str( const string& s, string default_value ) { return s; }

	/// convert data to string
	XO_API string to_str( float value );
	XO_API string to_str( double value );
	XO_API string to_str( bool value );
	XO_API string to_str( int value );
	XO_API string to_str( unsigned int value );
	XO_API string to_str( long value );
	XO_API string to_str( unsigned long value );
	XO_API string to_str( size_t value );
	XO_API string to_str( const char* value );
	inline string to_str( string value ) { return value; }
}
