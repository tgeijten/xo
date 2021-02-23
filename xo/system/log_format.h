#pragma once

#include "xo/string/string_type.h"
#include "xo/container/flag_set.h"
#include "xo/system/system_tools.h"
#include "xo/system/log_level.h"

namespace xo
{
	namespace log
	{
		enum class format {
			show_time, show_level
		};

		using format_flags = xo::flag_set<format>;

		inline string get_log_prefix( format_flags fmt, level l ) {
			string s;
			if ( fmt.get<format::show_time>() )
				s += get_date_time_str( "%H:%M:%S " );
			if ( fmt.get<format::show_level>() )
				s += level_names_short[ (int)l ], s += ' ';
			return s;
		}
	}
}
