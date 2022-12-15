#pragma once

#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include "xo/container/vector_type.h"

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

	XO_API string to_str( const void* ptr );
	template< typename T > string to_str( const T* ptr ) { return to_str( reinterpret_cast<const void*>( ptr ) ); }

	/// convert elements in a container to a string
	template< typename C >
	string container_to_str( const C& cont ) {
		string s = "[ ";
		for ( const auto& e : cont )
			s += to_str( e ) + ' ';
		s += "]";
		return s;
	}

	/// convert vector of elements to string
	template< typename T >
	string to_str( const vector<T>& v ) { return container_to_str( v ); }
}
