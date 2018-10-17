#pragma once

#include "xo/string/string_type.h"
#include "xo/system/platform.h"
#include "xo/numerical/math.h"
#include <vector>
#include <cctype>

#define xo_varstr( var_ ) ( ::std::string( #var_ ) + '=' + ::xo::to_str( ( var_ ) ) )

namespace xo
{
	/// get left n characters; if n < 0, get string WITHOUT the left n characters
	inline string left_str( const string& str, int n )
	{ if ( n >= 0 ) return str.substr( 0, size_t( n ) ); else return str.substr( 0, size_t( max( 0, int(str.size()) + n ) ) ); }

	/// get middle n characters, starting from pos
	inline string mid_str( const string& str, index_t pos, size_t n = string::npos ) { return str.substr( pos, n ); }

	/// get right n characters; if n < 0, get string WITHOUT the right n characters
	inline string right_str( const string& str, int n )
	{ if ( n >= 0 ) return str.substr( str.size() - n, string::npos ); else return str.substr( size_t( -n ), string::npos ); }

	/// get index of a substring in a string
	inline index_t in_str( const string& str, const string& substr, index_t p = 0 ) { return str.find( substr, p ); }

	/// check if a string ends with a string
	inline bool str_begins_with( const string& str, const string& substr ) { return str.find( substr ) == 0; }

	/// check if a string ends with a string
	inline bool str_ends_with( const string& str, const string& substr )
	{ return str.size() >= substr.size() && ( str.find( substr ) == str.size() - substr.size() ); }

	/// remove leading and trailing spaces and newlines
	XO_API string trim_str( const string& str, const char* trim_chars = " \t\r\n\f\v" );

	/// remove trailing spaces and newlines
	XO_API string trim_right_str( const string& str, const char* trim_chars = " \t\r\n\f\v" );

	/// split a string into a vector of strings
	XO_API std::vector< string > split_str( const string& s, const string& sep_chars );

	/// get the string left / right of a delimiter
	XO_API string left_of_str( const string& s, const string& sep_chars );
	XO_API string right_of_str( const string& s, const string& sep_chars );

	/// replace a substring in a string
	XO_API string& replace_str( string& s, const string& find_str, const string& replace_with );

	inline string to_lower( string& s ) { for ( char& c : s ) c = std::tolower( c ); return s; }
	inline string to_upper( string& s ) { for ( char& c : s ) c = std::toupper( c ); return s; }

	/// split string into trimmed key / value pair
	XO_API std::pair< string, string > make_key_value_str( const string& s, const string& sep_char = "=" );

	XO_API string concatenate_str( std::initializer_list< string > lst, const string& delim = "" );

	/// get formatted string (printf style)
	XO_API string stringf( const char* format, ... );

	/// match pattern (glob, i.e. name* or name?)
	XO_API bool pattern_match( const string& str, const string& pattern );

	/// match any of the strings
	XO_API bool str_equals_any_of( const string& str, std::initializer_list< const char* > str_list );

	/// set precision used in to_str calls, returns previous precision
	XO_API int set_to_str_precision( int p );
	XO_API int to_str_precision();

	/// get a string between quotes
	XO_API string encode_char( char c );

	/// get a string between quotes
	XO_API char decode_char( const char* buf, size_t buf_size, int* len );

	/// get a string between quotes
	XO_API string quoted( const string& s );

	/// get a string between quotes
	XO_API bool needs_quotes( const string& s, const char* special_chars = "" );

	/// get a string between quotes
	XO_API string try_quoted( const string& s, const char* special_chars = "" );

	/// get a string between quotes
	XO_API string try_unquoted( const string& s );
}
