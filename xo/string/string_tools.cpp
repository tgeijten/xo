#include "string_tools.h"

#ifdef XO_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <shlwapi.h> // used by glob_match
#	undef small // windows defines small :-S
#	pragma comment( lib, "shlwapi.lib" )
#else
#	include <fnmatch.h>
#endif

#include <stdarg.h>
#include <memory>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cctype>

#include "xo/system/error_code.h"
#include "xo/numerical/math.h"
#include "xo/system/assert.h"

namespace xo
{
	int to_str_precision_value = 4;

	string left_str( const string& str, int n )
	{
		if ( n >= 0 )
			return str.substr( 0, size_t( n ) );
		else return str.substr( 0, size_t( max( 0, int( str.size() ) + n ) ) );
	}

	string mid_str( const string& str, index_t pos, size_t n )
	{
		return str.substr( pos, n );
	}

	string right_str( const string& str, int n )
	{
		if ( n >= 0 )
			return str.substr( str.size() - n, string::npos );
		else return str.substr( size_t( -n ), string::npos );
	}

	index_t in_str( const string& str, string_view substr, index_t p /*= 0 */ )
	{
		return str.find( substr, p );
	}

	index_t find_nth_str( const string& str, string_view substr, size_t n )
	{
		index_t idx = 0;
		for ( ; n > 0 && idx != string::npos; --n )
			idx = str.find( substr, idx > 0 ? idx + 1 : idx );
		return idx;
	}

	bool str_begins_with( const string& str, string_view substr )
	{
		return str.compare( 0, substr.size(), substr ) == 0;
	}

	bool str_begins_with( const string& str, string_view substr, index_t start_pos )
	{
		return start_pos < str.size() && str.compare( start_pos, substr.size(), substr ) == 0;
	}

	bool str_begins_with( const string& str, char c )
	{
		return !str.empty() && str.front() == c;
	}

	bool str_ends_with( const string& str, string_view substr )
	{
		return str.size() >= substr.size()
			&& str.compare( str.size() - substr.size(), substr.size(), substr ) == 0;
	}

	bool str_ends_with( const string& str, char c )
	{
		return !str.empty() && str.back() == c;
	}

	bool str_ends_with_any_of( const string& str, std::initializer_list<string_view> str_list )
	{
		for ( const auto& s : str_list )
			if ( str_ends_with( str, s ) )
				return true;
		return false;
	}

	bool str_contains( const string& str, string_view substr )
	{
		return str.find( substr ) != string::npos;
	}

	bool str_contains( const string& str, char c )
	{
		return str.find( c ) != string::npos;
	}

	string trim_str( const string& s, const char* space_chars )
	{
		auto left = s.find_first_not_of( space_chars );
		if ( left == string::npos ) return string(); // string has no non-whitespace characters
		auto right = s.find_last_not_of( space_chars );
		return s.substr( left, 1 + right - left );
	}

	string trim_left_str( const string& str, const char* trim_chars )
	{
		auto left = str.find_first_not_of( trim_chars );
		return left != string::npos ? str.substr( left ) : string();
	}

	string trim_right_str( const string& s, const char* space_chars )
	{
		auto right = s.find_last_not_of( space_chars );
		return s.substr( 0, 1 + right );
	}

	vector< string > split_str( const string& s, string_view sep_chars )
	{
		std::vector< string > strings;
		size_t ofs = s.find_first_not_of( sep_chars, 0 );
		while ( ofs != string::npos ) {
			size_t ofsend = s.find_first_of( sep_chars, ofs );
			strings.push_back( s.substr( ofs, ofsend - ofs ) );
			ofs = s.find_first_not_of( sep_chars, ofsend );
		}
		return strings;
	}

	vector< string_view > split_str( const string_view& s, string_view sep_chars )
	{
		std::vector< string_view > strings;
		size_t ofs = s.find_first_not_of( sep_chars, 0 );
		while ( ofs != string::npos ) {
			size_t ofsend = s.find_first_of( sep_chars, ofs );
			strings.push_back( s.substr( ofs, ofsend - ofs ) );
			ofs = s.find_first_not_of( sep_chars, ofsend );
		}
		return strings;
	}

	std::pair< string, string > split_str_at_first( const string& s, string_view sep_chars )
	{
		if ( auto pos = s.find_first_of( sep_chars ); pos != string::npos )
			return { s.substr( 0, pos ), s.substr( pos + 1 ) };
		else return { s, string() };
	}

	std::pair< string, string > split_str_at_last( const string& s, string_view sep_chars )
	{
		if ( auto pos = s.find_last_of( sep_chars ); pos != string::npos )
			return { s.substr( 0, pos ), s.substr( pos + 1 ) };
		else return { s, string() };
	}

	void remove_str( string& s, string_view find_str )
	{
		xo_assert( !find_str.empty() );
		for ( auto pos = s.find( find_str ); pos != string::npos; pos = s.find( find_str, pos ) )
			s.erase( pos, find_str.size() );
	}

	string remove_str( const string& s, string_view find_str )
	{
		return remove_str( string( s ), find_str );
	}

	string remove_str( string&& s, string_view find_str )
	{
		remove_str( s, find_str );
		return std::move( s );
	}

	void replace_str( string& s, string_view find_str, string_view replace_with )
	{
		xo_assert( !find_str.empty() );
		for ( auto pos = s.find( find_str ); pos != string::npos; pos = s.find( find_str, pos + replace_with.size() ) )
			s.replace( pos, find_str.size(), replace_with );
	}

	string replace_str( const string& s, string_view find_str, string_view replace_with )
	{
		return replace_str( string( s ), find_str, replace_with );
	}

	string replace_str( string&& s, string_view find_str, string_view replace_with )
	{
		replace_str( s, find_str, replace_with );
		return std::move( s );
	}

	void replace_char( string& s, const char find_char, const char replace_with )
	{
		for ( auto& c : s )
			if ( c == find_char )
				c = replace_with;
	}

	string replace_char( string&& s, const char find_char, const char replace_with )
	{
		replace_char( s, find_char, replace_with );
		return std::move( s );
	}

	string to_lower( string s )
	{
		for ( char& c : s )
			c = std::tolower( c );
		return s;
	}

	string to_upper( string s )
	{
		for ( char& c : s )
			c = std::toupper( c );
		return s;
	}

	string capitalize_str( string s, string_view sep_chars )
	{
		bool new_word = true;
		for ( char& c : s ) {
			c = new_word ? std::toupper( c ) : std::tolower( c );
			new_word = sep_chars.find( c ) != string::npos;
		}
		return s;
	}

	string convert_all_caps_str( string s, string_view sep_chars )
	{
		bool new_word = true;
		for ( char& c : s ) {
			if ( !new_word )
				c = std::tolower( c );
			new_word = sep_chars.find( c ) != string::npos;
		}
		return s;
	}

	void replace_special_chars( string& s )
	{
		static const std::vector<std::pair<string, string>> fixes = {
			{ "ÁÀ", "A" }, { "Ä", "Ae" }, { "ÜÛÙ", "U" },
			{ "áà", "a" }, { "ä", "ae" }, { "ç", "c" },
			{ "ëêéè", "e" }, { "ï", "i" }, { "ñ", "n" },
			{ "óö", "o" }, { "ûù", "u" }, { "ü", "ue" }
		};

		for ( auto& [chars, subst] : fixes )
			for ( auto& c : chars )
				replace_str( s, std::string{ c }, subst );
	}

	size_t copy_str( const string& source, char* target_buf, size_t buf_size )
	{
		if ( buf_size > 0 )
		{
			auto n = source.copy( target_buf, buf_size - 1 );
			target_buf[n] = '\0';
			return n;
		}
		else return 0;
	}

	std::pair< string, string > make_key_value_str( const string& s, const string& sep_char )
	{
		auto pos = s.find_first_of( sep_char.c_str() );
		if ( pos == string::npos )
			return make_pair( s, string( "" ) );
		else return make_pair( trim_str( s.substr( 0, pos ) ), trim_str( mid_str( s, pos + 1 ) ) );
	}

	string concat_str( std::initializer_list<string> string_list, const string& delim )
	{
		string str;
		bool first = true;
		for ( auto& element : string_list )
		{
			if ( !element.empty() )
			{
				if ( !first ) str += delim; else first = false;
				str += element;
			}
		}
		return str;
	}

	string concat_str( const string& s1, const string& s2, const char delim )
	{
		if ( s1.empty() )
			return s2;
		else if ( s2.empty() )
			return s1;
		else return s1 + delim + s2;
	}

	string stringf( const char* format, ... )
	{
#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4996 )
#endif
		va_list args;
		va_start( args, format );
		char buf[256];
		vsnprintf( buf, sizeof( buf ), format, args );
		va_end( args );
		return string( buf );
#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
	}

	bool str_equals_any_of( const string& str, std::initializer_list<const char*> str_list )
	{
		for ( const char* c : str_list )
			if ( str == c )
				return true;
		return false;
	}

	int set_to_str_precision( int p )
	{ int old = to_str_precision_value; to_str_precision_value = p; return old; }

	int to_str_precision()
	{ return to_str_precision_value; }

	string encode_char( char c )
	{
		if ( c == '\"' )
			return "\\\"";
		else if ( c == '\\' )
			return "\\\\";
		else if ( c < 32 )
		{
			switch ( c )
			{
			case '\r': return "\\r";
			case '\n': return "\\n";
			case '\t': return "\\t";
			default: return stringf( "\\%03o", (int)c );
			}
		}
		else return string( 1, c ); // nothing to encode
	}

	char decode_char( const char* buf, size_t buf_size, int* len )
	{
		xo_error_if( buf_size == 0, "Invalid buffer size" );
		*len = 0;
		if ( *buf != '\\' || buf_size == 1 )
			return *buf;

		*len = 2;
		switch ( *( ++buf ) )
		{
		case 0: return '?'; // this is an error
		case '\\': return '\\';
		case '\"': return '\"';
		case 'r': return '\r';
		case 'n': return '\n';
		case 't': return '\t';
		default:
		{
			// read oct digits
			int value = 0;
			int max_len = std::min( (int)buf_size - 1, 3 );
			for ( *len = 1; *len <= max_len; ++( *len ) )
			{
				if ( *buf >= '0' && *buf < '8' )
					value = value * 8 + *buf++ - '0';
				else break;
			}
			if ( *len > 1 && value > 0 && value < 32 )
				return char( value ); // we read a valid char
			else return '?'; // no valid char could be read
		}
		}
	}

	string quoted( const string& s )
	{
		string sout = "\"";
		for ( const char& c : s )
		{
			if ( c == '\"' || c == '\\' || c < 32 )
				sout += encode_char( c );
			else sout += c;
		}
		sout += "\"";
		return sout;
	}

	bool needs_quotes( const string& s, const char* special_chars )
	{
		for ( const char& c : s )
			if ( c < 33 || c == '\"' || strchr( special_chars, c ) )
				return true;
		return s.empty();
	}

	string try_quoted( const string& s, const char* special_chars )
	{
		if ( needs_quotes( s, special_chars ) )
			return quoted( s );
		else return s;
	}

	string try_unquoted( const string& s )
	{
		if ( s.empty() || s.front() != '\"' || s.back() != '\"' )
			return s; // no quotes

		string sout;
		for ( auto it = s.begin() + 1; *it != '\"'; ++it )
		{
			if ( *it == '\\' )
			{
				int len;
				sout += decode_char( &( *it ), s.end() - it, &len );
				it += ( len - 1 );
			}
			else sout += *it;
		}
		return sout;
	}

	bool pattern_match( const string& str, const string& pattern )
	{
#ifdef XO_COMP_MSVC
		xo_assert_msg( str.find_first_of( ";" ) == string::npos, "pattern_match patterns cannot contain ';' on MSVC" );
		return PathMatchSpecEx( str.c_str(), pattern.c_str(), PMSF_NORMAL ) == S_OK;
#else
		return fnmatch( pattern.c_str(), str.c_str(), FNM_NOESCAPE ) == 0;
#endif
	}

	void append_str( string& str, const string& append, const string& delim )
	{
		if ( !str.empty() && !str_ends_with( str, delim ) )
			str += delim;
		str += append;
	}

	string get_filename_ext( const string& str )
	{
		size_t n = str.find_last_of( '.' );
		if ( n != string::npos ) {
			string ext = str.substr( n + 1 );
			if ( ext.find_last_of( "/\\" ) == string::npos ) // check if not part of folder
				return ext;
		}
		return string(); // no extension found
	}

	string get_filename_without_ext( const string& str )
	{
		auto ext_len = get_filename_ext( str ).size();
		if ( ext_len > 0 ) ++ext_len; // add dot
		return str.substr( 0, str.size() - ext_len );
	}

	string get_filename_folder( const string& str )
	{
		size_t n = str.find_last_of( "/\\" );
		if ( n != string::npos ) return str.substr( 0, n + 1 );
		else return str;
	}

	string get_filename_without_folder( const string& str )
	{
		size_t n = str.find_last_of( "/\\" );
		if ( n != string::npos ) return str.substr( n + 1, string::npos );
		else return str;
	}
}
