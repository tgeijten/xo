#include "char_stream.h"
#include "xo/string/string_tools.h"
#include "xo/system/system_tools.h"
#include "xo/filesystem/filesystem.h"
#include "xo/filesystem/path.h"
#include <cstring>
#include <algorithm>

namespace xo
{
	char_stream::char_stream( const char* buf, string delim_chars, string quote_chars, std::vector<string> operators ) :
		delimiters_( delim_chars ),
		quotations_( quote_chars )
	{
		initialize( buf, strlen( buf ) );
		set_operators( operators );
	}

	char_stream::char_stream( string&& other, string delim_chars, string quote_chars, std::vector< string > operators ) :
		str_buffer( std::move( other ) ),
		delimiters_( delim_chars ),
		quotations_( quote_chars )
	{
		initialize( str_buffer.c_str(), str_buffer.size() );
		set_operators( operators );
	}

	void char_stream::set_operators( std::vector< string > operators )
	{
		// sort operators based on length, from large to small
		operators_ = std::move( operators );
		std::sort( operators_.begin(), operators_.end(), [&]( const string& a, const string& b ) { return a.size() > b.size(); } );
	}

	void char_stream::set_delimiter_chars( string delimiter_chars )
	{
		delimiters_ = std::move( delimiter_chars );
	}

	void char_stream::set_quotation_chars( string quotation_chars )
	{
		quotations_ = std::move( quotation_chars );
	}

	xo::string char_stream::get_line()
	{
		if ( !test_eof() )
		{
			size_t len = strcspn( cur_pos, "\r\n" );
			string s = string( cur_pos, len );
			if ( len > 0 ) {
				// only process end_pos when s is not empty or it will fail incorrectly (e.g. empty comment)
				cur_pos_end = const_cast<char*>( cur_pos ) + len;
				process_end_pos();
			}
			return s;
		}
		else return "";
	}

	xo::string char_stream::get_token()
	{
		skip_delimiters();
		string s;
		cur_pos_end = const_cast<char*>( cur_pos );
		while ( good() )
		{
			if ( cur_pos_end == buffer_end || strchr( delimiters_.c_str(), *cur_pos_end ) ) // check for delimiter
			{
				// end of buffer or delimiter
				s += string( cur_pos, size_t( cur_pos_end - cur_pos ) );
				cur_pos = cur_pos_end;
				break;
			}
			else if ( strchr( quotations_.c_str(), *cur_pos_end ) ) // check for quote
			{
				// this is a part between quotes and must be decoded
				cur_pos = cur_pos_end;
				char quote_char = getc();
				while ( good() )
				{
					char c = peekc();
					if ( c == quote_char ) // end of quote
					{
						getc();
						break;
					}
					else if ( c == '\\' )
					{
						int len;
						s += decode_char( cur_pos, buffer_end - cur_pos, &len );
						for ( ; len > 0; --len ) getc();
					}
					else s += getc();
				}
				cur_pos_end = const_cast<char*>( cur_pos );
			}
			else if ( auto opstr = check_operator( cur_pos_end ) ) // check for operator
			{
				if ( cur_pos == cur_pos_end && s.empty() )
				{
					s = *opstr; // token is an operator
					cur_pos += opstr->length();
				}
				else
				{
					// token is everything up to operator
					s += string( cur_pos, size_t( cur_pos_end - cur_pos ) );
					cur_pos = cur_pos_end;
				}
				break;
			}
			else ++cur_pos_end;
		}
		test_eof();
		return s;
	}

	size_t char_stream::line_number() const
	{
		size_t num = 1;
		for ( const char* p = buffer; p != cur_pos; ++p )
			num += ( *p == '\n' );
		return num;
	}

	bool char_stream::seek( const string& s )
	{
		if ( good() )
		{
			if ( const char* p = strstr( cur_pos, s.c_str() ) )
			{
				cur_pos = p;
				return true;
			}
		}

		buffer_flags.set< fail_flag >();
		return false;
	}

	bool char_stream::seek_past( const string& s )
	{
		if ( seek( s ) )
		{
			cur_pos += s.length();
			test_eof();
			return true;
		}
		else return false;
	}

	void char_stream::initialize( const char* b, size_t len )
	{
		// setup buffer pointers
		xo_assert( b != 0 );
		cur_pos = buffer = b;
		cur_pos_end = nullptr;
		buffer_end = buffer + len;
	}

	bool char_stream::try_get( const string& s )
	{
		if ( strncmp( cur_pos, s.c_str(), s.length() ) == 0 )
		{
			cur_pos += s.length();
			return true;
		}
		else return false;
	}

	bool char_stream::try_get( char c )
	{
		if ( *cur_pos == c )
		{
			++cur_pos;
			return true;
		}
		else return false;
	}

	const string* char_stream::check_operator( const char* s )
	{
		for ( auto& op : operators_ )
		{
			if ( strncmp( op.c_str(), s, op.size() ) == 0 )
				return &op;
		}
		return nullptr;
	}
}
