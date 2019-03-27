#pragma once

#include <vector>

#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include "xo/system/assert.h"
#include "xo/system/error_code.h"
#include "xo/container/flag_set.h"
#include "xo/utility/optional.h"

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	/// blazing-fast class for reading text stream-style
	class XO_API char_stream // TODO: rename this to char_parser or something
	{
	public:
		/// construct char_stream using given zero terminated char buffer
		explicit char_stream( const char* buf, string delim_chars = whitespace_characters, string quote_chars = "\"", std::vector< string > operators = {} );

		/// construct char_stream from rvalue string
		explicit char_stream( string&& other, string delim_chars = whitespace_characters, string quote_chars = "\"", std::vector< string > operators = {} );

		/// construct char_stream with contents read from file
		explicit char_stream( const path& filename, string delim_chars = whitespace_characters, string quote_chars = "\"", std::vector< string > operators = {} );

		~char_stream() {}

		/// initialize operators
		void set_operators( std::vector< string > operators );
		void set_delimiter_chars( string delimiter_chars );
		void set_quotation_chars( string quotation_chars );

		/// read PODs
		char_stream& operator>>( float& v ) { skip_delimiters(); v = strtof( cur_pos, &cur_pos_end ); process_end_pos(); return *this; }
		char_stream& operator>>( double& v ) { skip_delimiters(); v = strtod( cur_pos, &cur_pos_end ); process_end_pos(); return *this; }
		char_stream& operator>>( long& v ) { skip_delimiters(); v = strtol( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }
		char_stream& operator>>( long long& v ) { skip_delimiters(); v = strtoll( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }
		char_stream& operator>>( unsigned long& v ) { skip_delimiters(); v = strtoul( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }
		char_stream& operator>>( unsigned long long& v ) { skip_delimiters(); v = strtoull( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }
		char_stream& operator>>( int& v ) { skip_delimiters(); v = (int)strtol( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }
		char_stream& operator>>( unsigned int& v ) { skip_delimiters(); v = (unsigned int)strtoul( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }
		char_stream& operator>>( short& v ) { skip_delimiters(); v = (short)strtol( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }
		char_stream& operator>>( unsigned short& v ) { skip_delimiters(); v = (unsigned short)strtoul( cur_pos, &cur_pos_end, radix ); process_end_pos(); return *this; }

		/// read optional variable
		template< typename T > char_stream& operator>>( optional< T >& v ) { T tmp; *this >> tmp; if ( !fail() ) v = tmp; else v.reset(); return *this; }

		/// read token
		char_stream& operator>>( string& s ) { s = get_token(); return *this; }

		string get_line();
		string get_token();

		char getc() { if ( !test_eof() ) return *cur_pos++; else return '\0'; }
		char peekc() { if ( !test_eof() ) return *cur_pos; else return '\0'; }

		bool seek( const string& s );
		bool seek_past( const string& s );
		bool try_get( const string& s );

		bool good() { return !buffer_flags.any(); }
		bool eof() { return buffer_flags.get< eof_flag >(); }
		bool fail() { return buffer_flags.get< fail_flag >(); }

		size_t line_number() const;

	private:
		void initialize( const char* b, size_t len );
		const string* check_operator( const char* s );
		void skip_delimiters() { while ( !test_eof() && strchr( delimiters_.c_str(), *cur_pos ) ) ++cur_pos; }
		bool test_eof() { if ( cur_pos == buffer_end ) { buffer_flags.set< eof_flag >(); return true; } else return false; }
		void process_end_pos() { if ( cur_pos == cur_pos_end ) buffer_flags.set< fail_flag >(); else cur_pos = cur_pos_end; }

		int radix = 10;
		string str_buffer;
		const char* buffer;
		const char* cur_pos;
		char* cur_pos_end;
		const char* buffer_end;

		std::vector< string > operators_;
		string delimiters_;
		string quotations_;

		enum buffer_flag { fail_flag, eof_flag };
		flag_set< buffer_flag > buffer_flags;
	};
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
