#include "string_cast.h"

#include "xo/system/version.h"
#include "xo/string/string_tools.h"
#include "xo/time/time.h"
#include "xo/filesystem/path.h"

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
		char buf[32];
		std::snprintf( buf, sizeof( buf ), "%g", value );
		return string( buf );
	}

	string to_str( double value )
	{
		char buf[32];
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

	string to_str( const void* ptr )
	{
		return stringf( "%p", ptr );
	}

	bool from_str( const string& s, version& ver )
	{
		auto [numbers, postfix] = split_str_at_first( s, whitespace_characters );
		if ( auto vs = split_str( numbers, "." ); vs.size() >= 3 )
		{
			if ( from_str( vs[0], ver.major_ )
				&& from_str( vs[1], ver.minor_ )
				&& from_str( vs[2], ver.patch_ ) )
			{
				if ( vs.size() >= 4 )
					from_str( vs[3], ver.build_ );
				ver.postfix_ = postfix;
				return true;
			}
		}
		return false;
	}

	string to_str( const version& v )
	{
		string s = stringf( "%d.%d.%d", v.major_, v.minor_, v.patch_ );
		if ( v.build_ > 0 ) s += stringf( ".%d", v.build_ );
		if ( !v.postfix_.empty() ) s += v.postfix_;
		return s;
	}

	bool from_str( const string& s, path& v )
	{
		v = path( s );
		return true;
	}

	string to_str( const path& p )
	{
		return p.str();
	}

	string to_str( const time& v, double hide_decimals_after_seconds )
	{
		string str;
		auto sd = v.seconds<double>();
		auto si = size_t( sd );
		auto h = si / 3600;
		if ( h > 0 ) {
			str += stringf( "%d:", h );
			si -= h * 3600;
		}
		auto m = si / 60;
		if ( h > 0 || m > 0 ) {
			str += stringf( str.empty() ? "%d:" : "%02d:", m );
			si -= m * 60;
		}
		if ( h > 0 || m > 0 ) // show whole seconds
			str += stringf( "%02d", si );
		else if ( sd >= hide_decimals_after_seconds ) // show whole seconds with zero minutes
			str += stringf( "0:%02d", si );
		else // show decimal points
			str += stringf( "%.2f", sd );

		return str;
	}

	bool from_str( const string& s, time& v )
	{
		if ( double d; from_str( s, d ) ) { v = time_from_seconds( d ); return true; }
		else return false;
	}
}
