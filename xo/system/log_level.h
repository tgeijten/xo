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

		static constexpr char* const level_names_short[] =
		{ "", "TRC", "DBG", "INF", "WRN", "ERR", "CRT", "" };

		static constexpr char* const level_names_full[] =
		{ "", "trace", "debug", "info", "warning", "error", "critical", "" };
	}
}
