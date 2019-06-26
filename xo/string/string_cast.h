#pragma once

#include "xo/system/xo_config.h"
#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include "xo/geometry/angle.h"
#include "xo/geometry/quat_type.h"
#include "xo/geometry/vec3_type.h"
#include "xo/numerical/bounds.h"

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

	template< angle_unit U, typename T > string to_str( const angle_<U, T>& a ) { return to_str( a.value ); }
	template< angle_unit U, typename T > bool from_str( const string& str, angle_<U, T>& v ) { return from_str( str, v.value ); }

	template< typename T > string to_str( const quat_<T>& q ) { return to_str( q.w ) + ' ' + to_str( q.x ) + ' ' + to_str( q.y ) + ' ' + to_str( q.z ); }

	template< typename T > string to_str( const vec3_<T>& v ) { return to_str( v.x ) + ' ' + to_str( v.y ) + ' ' + to_str( v.z ); }

	template< typename T > string to_str( const bounds<T>& v ) { return to_str( v.lower ) + ' ' + to_str( v.upper ); }
	template< typename T > bool from_str( const string& s, bounds<T>& v ) {
		if ( from_str( s, v.lower ) ) {
			if ( auto p = s.find( ".." ); p != string::npos )
				return from_str( s.substr( p + 2 ), v.upper );
			else { v.upper = v.lower; return true; }
		}
		else return false;
	}
}
