#include "log_sink.h"
#include <iostream>
#include "system_tools.h"

namespace xo
{
	
	log::sink::sink( level l ) : log_level_( l )
	{
		xo::log::add_sink( this );
	}

	log::sink::~sink()
	{
		xo::log::remove_sink( this );
	}

	void log::sink::try_send_log_message( level l, const string& msg )
	{
		if ( test_log_level( l ) )
			send_log_message( l, msg );
	}

	void log::sink::set_log_level( level l )
	{
		log_level_ = l; xo::log::get_global_log_level();
	}

	xo::log::level log::sink::get_log_level()
	{
		return log_level_;
	}

	bool log::sink::test_log_level( level l )
	{
		return l >= log_level_;
	}

	log::stream_sink::stream_sink( level l, std::ostream& str ) : sink( l ), stream_( str )
	{}

	void log::stream_sink::send_log_message( level l, const string& msg )
	{
		// TODO: make the time prefix an option
		auto str = get_date_time_str( "%H:%M:%S " );
		stream_ << str << msg << std::endl;
		if ( l >= error_level )
			stream_.flush();
	}

	log::console_sink::console_sink( level l ) : stream_sink( l, std::cout )
	{}

	log::file_sink::file_sink( level l, const path& file ) : file_stream_( file.str() ), stream_sink( l, file_stream_ )
	{}

	void log::file_sink::send_log_message( level l, const string& msg )
	{
		if ( file_stream_.good() )
		{
			stream_sink::send_log_message( l, msg ); file_stream_.flush();
		}
	}

	bool log::file_sink::good()
	{
		return file_stream_.good();
	}
}
