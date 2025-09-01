#pragma once

namespace xo
{
	namespace log
	{
		enum class level {
			always = 0,
			trace = 1,
			debug = 2,
			info = 3,
			warning = 4,
			error = 5,
			critical = 6,
			never = 7
		};

		inline constexpr const char* level_names_short[] = {
			"", "TRC", "DBG", "INF", "WRN", "ERR", "CRT", ""
		};

		inline constexpr const char* level_names_full[] = {
			"", "trace", "debug", "info", "warning", "error", "critical", ""
		};

		inline level make_level( int l ) {
			if ( l < 0 ) l = 0; else if ( l > 7 ) l = 7;
			return level( l );
		}
	}
}
