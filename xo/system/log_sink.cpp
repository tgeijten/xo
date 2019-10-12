#include "log_sink.h"
#include <iostream>
#include "xo/filesystem/filesystem.h"
#include "system_tools.h"

#ifdef XO_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

namespace xo
{
	namespace log
	{
		sink::sink( level l ) : log_level_( l )
		{
			add_sink( this );
		}

		sink::~sink()
		{
			remove_sink( this );
		}

		void sink::try_send_log_message( level l, const string& msg )
		{
			if ( test_log_level( l ) )
				send_log_message( l, msg );
		}

		void sink::set_log_level( level l )
		{
			log_level_ = l;
			update_global_log_level();
		}

		level sink::get_log_level()
		{
			return log_level_;
		}

		bool sink::test_log_level( level l )
		{
			return l >= log_level_;
		}

		stream_sink::stream_sink( level l, std::ostream& str ) :
			sink( l ),
			stream_( str )
		{}

		void stream_sink::send_log_message( level l, const string& msg )
		{
			// #todo: make the time prefix an option
			auto str = get_date_time_str( "%H:%M:%S " );
			stream_ << str << msg << std::endl;
		}

		void stream_sink::flush()
		{
			stream_.flush();
		}

		console_sink::console_sink( level l ) : stream_sink( l, std::cout )
		{}

		void console_sink::send_log_message( level l, const string & msg )
		{
			auto str = get_date_time_str( "%H:%M:%S " );
			stream_ << str;
			stream_.flush();

#ifdef XO_COMP_MSVC
			// set console color
			HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );
			CONSOLE_SCREEN_BUFFER_INFO info;
			GetConsoleScreenBufferInfo( h, &info );
			auto bg = info.wAttributes & ( BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY );
			switch ( l )
			{
			case xo::log::trace_level:
			case xo::log::debug_level:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | bg );
				break;
			case xo::log::info_level:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | bg );
				break;
			case xo::log::warning_level:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | bg );
				break;
			case xo::log::error_level:
			case xo::log::critical_level:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_INTENSITY | bg );
				break;
			}
#endif
			// output text
			stream_ << msg << std::endl;

#ifdef XO_COMP_MSVC
			// restore color
			SetConsoleTextAttribute( h, info.wAttributes );
#endif
		}

		file_sink::file_sink( level l, const path& file ) :
			stream_sink( l, file_stream_ )
		{
			if ( file.has_parent_path() )
				create_directories( file.parent_path() );
			file_stream_.open( file.str() );
		}

		void file_sink::send_log_message( level l, const string& msg )
		{
			if ( file_stream_.good() )
				stream_sink::send_log_message( l, msg );
		}

	}
}
