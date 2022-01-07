#pragma once

#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include "xo/container/vector_type.h"
#include "xo/container/pair_type.h"
#include "xo/string/string_cast.h"

#include <initializer_list>

#define xo_varstr( var_ ) ( ::std::string( #var_ ) + '=' + ::xo::to_str( ( var_ ) ) )

namespace xo
{
	/// get left n characters; if n is negative, get string WITHOUT the right n characters
	XO_API string left_str( const string& str, int n );

	/// get middle n characters, starting from pos
	XO_API string mid_str( const string& str, index_t pos, size_t n = string::npos );

	/// get right n characters; if n is negative, get string WITHOUT the left n characters
	XO_API string right_str( const string& str, int n );

	/// get index of a substring in a string
	XO_API index_t in_str( const string& str, const string& substr, index_t pos = 0 );

	/// check if a string begins with a string
	XO_API bool str_begins_with( const string& str, const string& substr );

	/// check if a string begins with a string
	XO_API bool str_begins_with( const string& str, const string& substr, index_t pos );

	/// check if a string begins with a string
	XO_API bool str_begins_with( const string& str, char c );

	/// check if a string ends with a string
	XO_API bool str_ends_with( const string& str, const string& substr );

	/// check if a string ends with a string
	XO_API bool str_ends_with( const string& str, char c );

	/// check if a string contains a string
	XO_API bool str_contains( const string& str, const string& substr );

	/// check if a string contains a string
	XO_API bool str_contains( const string& str, char c );

	/// remove leading and trailing characters
	XO_API string trim_str( const string& str, const char* trim_chars = " \t\r\n\f\v" );

	/// remove leading characters
	XO_API string trim_left_str( const string& str, const char* trim_chars = " \t\r\n\f\v" );

	/// remove trailing characters
	XO_API string trim_right_str( const string& str, const char* trim_chars = " \t\r\n\f\v" );

	/// split a string into a vector of strings
	XO_API vector< string > split_str( const string& s, const string& sep_chars );

	/// split string into pair at first occurrence of sep_char, second is empty if not occurring
	XO_API std::pair< string, string > split_str_at_first( const string& s, const string& sep_chars );

	/// split string into pair at first occurrence of sep_char, second is empty if not occurring
	XO_API std::pair< string, string > split_str_at_last( const string& s, const string& sep_chars );

	/// replace a substring in a string
	XO_API string& replace_str( string& s, const string& find_str, const string& replace_with );

	/// replace a substring in a temporary string
	XO_API string replace_str( string&& s, const string& find_str, const string& replace_with );

	/// replace a character in a string
	XO_API string& replace_char( string& s, const char find_char, const char replace_with );

	/// replace a character in a string
	XO_API string replace_char( string&& s, const char find_char, const char replace_with );

	/// convert string to lower case
	XO_API string to_lower( string s );

	/// convert string to upper case
	XO_API string to_upper( string s );

	/// copy string to C-style null-terminated string, truncate if needed, returns number of characters copied
	XO_API size_t copy_str( const string& source, char* target_buf, size_t buf_size );

	/// split string into trimmed key / value pair
	XO_API std::pair< string, string > make_key_value_str( const string& s, const string& sep_char = "=" );

	/// get formatted string (printf style)
	XO_API string stringf( const char* format, ... );

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

	/// match pattern (glob, i.e. name* or name?)
	XO_API bool pattern_match( const string& str, const string& pattern );

	/// concatenate strings with delimiters
	XO_API string concatenate_str( std::initializer_list< string > lst, const string& delim = "" );

	/// concatenate container of strings with delimiters
	template< typename C >
	string concatenate_str( const C& container, const string& delim = "" ) {
		string str;
		bool first = true;
		for ( auto& element : container ) {
			if ( !element.empty() ) {
				if ( !first ) str += delim; else first = false;
				str += to_str( element );
			}
		}
		return str;
	}
}
