#pragma once

namespace xo
{
	namespace log
	{
		enum level {
			always_log_level = 0,
			trace_level,
			debug_level,
			info_level,
			warning_level,
			error_level,
			critical_level,
			never_log_level
		};
	}
}
