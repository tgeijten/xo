#include "char_stream.h"
#include "xo/string/string_tools.h"
#include "xo/system/system_tools.h"
#include "xo/filesystem/filesystem.h"
#include <cstring>

namespace xo
{
	char_stream::char_stream( const char* buf, const char* delim ) : delimiters_( delim )
	{
		init_buffer( buf, strlen( buf ) );
	}

	char_stream::char_stream( string&& other, const char* delim ) : str_buffer( std::move( other ) ), delimiters_( delim )
	{
		init_buffer( str_buffer.c_str(), str_buffer.size() );
	}

	char_stream::char_stream( const path& filename, const char* delim ) :
	char_stream( load_string( filename ), delim )
	{}

	xo::char_stream& char_stream::operator>>( string& s )
	{
		for ( cur_pos_end = const_cast<char*>( cur_pos ); *cur_pos_end && !isspace( *cur_pos_end ); ++cur_pos_end );
		s = string( cur_pos, size_t( cur_pos_end - cur_pos ) );
		process_end_pos();
		return *this;
	}

	xo::string char_stream::get_line()
	{
		size_t len = strcspn( cur_pos, "\r\n" );
		string s = string( cur_pos, len );
		cur_pos_end = const_cast<char*>(cur_pos) + len;
		process_end_pos();
		return s;
	}

	xo::string char_stream::get_token( const char* operators, const char* quotations )
	{
		string s;
		cur_pos_end = const_cast< char* >( cur_pos );
		while ( good() )
		{
			if ( cur_pos_end == buffer_end || strchr( delimiters_, *cur_pos_end ) )
			{
				// end of buffer or delimiter
				s += string( cur_pos, size_t( cur_pos_end - cur_pos ) );
				cur_pos = cur_pos_end;
				break;
			}
			else if ( strchr( quotations, *cur_pos_end ) )
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
			else if ( strchr( operators, *cur_pos_end ) )
			{
				// operator
				if ( cur_pos == cur_pos_end && s.empty() )
					s = *cur_pos_end++; // token is an operator
				else s += string( cur_pos, size_t( cur_pos_end - cur_pos ) );
				cur_pos = cur_pos_end;
				break;
			}
			else ++cur_pos_end;
		}
		skip_delimiters();
		test_eof();
		return s;
	}

	void char_stream::init_buffer( const char* b, size_t len )
	{
		xo_assert( b != 0 );
		cur_pos = buffer = b;
		cur_pos_end = nullptr;
		buffer_end = buffer + len;
		skip_delimiters();
	}

	xo::char_stream load_char_stream( const path& filename, const char* delimiters, error_code* ec )
	{
		return char_stream( load_string( filename, ec ), delimiters );
	}
}
