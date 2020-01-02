#pragma once

namespace xo
{
	namespace log
	{
		enum class level {
			always = 0,
			trace,
			debug,
			info,
			warning,
			error,
			critical,
			never
		};
	}
}
