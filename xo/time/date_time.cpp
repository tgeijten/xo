#include "date_time.h"

namespace xo
{
	std::int64_t date_time::timestamp() const
	{
		return
			year * 12 * 31 * 24 * 60 * 60 * 1000 +
			month * 31 * 24 * 60 * 60 * 1000 +
			( day - 1 ) * 24 * 60 * 60 * 1000 +
			hour * 60 * 60 * 1000 +
			minute * 60 * 1000 +
			second * 1000 +
			millisecond;
	}

}
