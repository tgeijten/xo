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
	}
}
