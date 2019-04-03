#include "string_cast.h"

#include "xo/system/version.h"
#include "xo/string/string_tools.h"

namespace xo
{

	bool from_str( const string& s, float& v )
	{
		char* p;
		v = std::strtof( s.c_str(), &p );
		return p != s.c_str();
	}

	bool from_str( const string& s, double& v )
	{
		char* p;
		v = std::strtod( s.c_str(), &p );
		return p != s.c_str();
	}

	bool from_str( const string& s, bool& v )
	{
		if ( str_equals_any_of( s, { "1", "true", "yes" } ) )
		{ v = true; return true; }
		else if ( str_equals_any_of( s, { "0", "false", "no" } ) )
		{ v = false; return true; }
		else return false; // could not extract boolean
	}

	bool from_str( const string& s, int& v )
	{
		char* p; 
		v = (int)( std::strtol( s.c_str(), &p, 10 ) );
		return p != s.c_str();
	}

	bool from_str( const string& s, unsigned int& v )
	{
		char* p;
		v = (unsigned int)( std::strtoul( s.c_str(), &p, 10 ) );
		return p != s.c_str();
	}

	bool from_str( const string& s, long& v )
	{
		char* p;
		v = std::strtol( s.c_str(), &p, 10 );
		return p != s.c_str();
	}

	bool from_str( const string& s, unsigned long& v )
	{
		char* p;
		v = std::strtoul( s.c_str(), &p, 10 );
		return p != s.c_str();
	}

	bool from_str( const string& s, long long& v )
	{
		char* p;
		v = std::strtoll( s.c_str(), &p, 10 );
		return p != s.c_str();
	}

	bool from_str( const string& s, unsigned long long& v )
	{
		char* p;
		v = std::strtoull( s.c_str(), &p, 10 );
		return p != s.c_str();
	}

	string to_str( float value )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", value );
		return string( buf );
	}

	string to_str( double value )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", value );
		return string( buf );
	}

	string to_str( bool value )
	{
		return string( value ? "1" : "0" );
	}

	string to_str( int value )
	{
		return std::to_string( value );
	}

	string to_str( unsigned int value )
	{
		return std::to_string( value );
	}

	string to_str( long value )
	{
		return std::to_string( value );
	}

	string to_str( unsigned long value )
	{
		return std::to_string( value );
	}

	string to_str( long long value )
	{
		return std::to_string( value );
	}

	string to_str( unsigned long long value )
	{
		return std::to_string( value );
	}

	string to_str( const version& v )
	{
		string s = stringf( "%d.%d.%d", v.major, v.minor, v.patch );
		if ( v.build > 0 ) s += stringf( ".%d", v.build );
		if ( !v.postfix.empty() ) s += ' ' + v.postfix;
		return s;
	}

	bool from_str( const string& s, version& ver )
	{
		auto[ numbers, postfix ] = split_str_at_first( s, whitespace_characters );
		if ( auto vs = split_str( numbers, "." ); vs.size() >= 3 )
		{
			if ( from_str( vs[ 0 ], ver.major )
				&& from_str( vs[ 1 ], ver.minor )
				&& from_str( vs[ 2 ], ver.patch ) )
			{
				if ( vs.size() >= 4 )
					from_str( vs[ 3 ], ver.build );
				ver.postfix = postfix;
				return true;
			}
		}
		return false;
	}

}
