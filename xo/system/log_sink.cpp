#include "log_sink.h"
#include <iostream>
#include "xo/filesystem/filesystem.h"
#include "system_tools.h"

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

		file_sink::file_sink( level l, const path& file ) : file_stream_(), stream_sink( l, file_stream_ )
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
