#include "log_sink.h"
#include <iostream>
#include "xo/filesystem/filesystem.h"
#include "system_tools.h"

#ifdef XO_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif
#include <mutex>

namespace xo
{
	namespace log
	{
		std::mutex g_console_mutex;

		sink::sink( level l, sink_mode m ) :
			log_level_( l ),
			sink_mode_( m ),
			thread_id_( std::this_thread::get_id() )
		{
			add_sink( this );
		}

		sink::~sink()
		{
			remove_sink( this );
		}

		bool sink::test_log_level( level l ) const
		{
			if ( sink_mode_ == sink_mode::all_threads )
				return l >= log_level_;
			else return l >= log_level_ && ( std::this_thread::get_id() == thread_id_ );
		}

		void sink::submit_log_message( level l, const string& msg )
		{
			if ( test_log_level( l ) )
				hande_log_message( l, msg );
		}

		void sink::set_log_level( level l )
		{
			log_level_ = l;
		}

		level sink::get_log_level()
		{
			return log_level_;
		}

		void sink::set_sink_mode( sink_mode m )
		{
			sink_mode_ = m;
			thread_id_ = std::this_thread::get_id();
		}

		stream_sink::stream_sink( std::ostream& str, level l, sink_mode m ) :
			sink( l, m ),
			stream_( str )
		{}

		void stream_sink::hande_log_message( level l, const string& msg )
		{
			// #todo: make the time prefix an option
			auto str = get_date_time_str( "%H:%M:%S " );
			stream_ << str << msg << std::endl;
		}

		void stream_sink::flush()
		{
			stream_.flush();
		}

		console_sink::console_sink( level l, sink_mode m ) :
			stream_sink( std::cout, l, m )
		{}

		void console_sink::hande_log_message( level l, const string& msg )
		{
			auto str = get_date_time_str( "%H:%M:%S " );

			std::scoped_lock lock( g_console_mutex );
			stream_ << str;

#ifdef XO_COMP_MSVC
			// set console color
			HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );
			CONSOLE_SCREEN_BUFFER_INFO info;
			GetConsoleScreenBufferInfo( h, &info );
			auto bg = info.wAttributes & ( BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY );
			switch ( l )
			{
			case xo::log::level::trace:
			case xo::log::level::debug:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | bg );
				break;
			case xo::log::level::info:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | bg );
				break;
			case xo::log::level::warning:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | bg );
				break;
			case xo::log::level::error:
			case xo::log::level::critical:
				SetConsoleTextAttribute( h, FOREGROUND_RED | FOREGROUND_INTENSITY | bg );
				break;
			}
#endif
			// output the log message
			// could not find performance benefits using '\n' instead of std::endl
			stream_ << msg << std::endl; 

#ifdef XO_COMP_MSVC
			// restore color
			SetConsoleTextAttribute( h, info.wAttributes );
#endif
		}

		file_sink::file_sink( const path& file, level l, sink_mode m ) :
			stream_sink( file_stream_, l, m )
		{
			if ( file.has_parent_path() )
				create_directories( file.parent_path() );
			file_stream_.open( file.str() );
		}

		void file_sink::hande_log_message( level l, const string& msg )
		{
			if ( file_stream_.good() )
				stream_sink::hande_log_message( l, msg );
		}
	}
}
