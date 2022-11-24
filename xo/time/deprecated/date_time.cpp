#include "date_time.h"

namespace xo
{
	uint64 date_time::timestamp() const
	{
		return
			uint64( year ) * 12 * 31 * 24 * 60 * 60 * 1000 +
			uint64(month )* 31 * 24 * 60 * 60 * 1000 +
			( uint64( day ) - 1 ) * 24 * 60 * 60 * 1000 +
			uint64(hour )* 60 * 60 * 1000 +
			uint64(minute )* 60 * 1000 +
			uint64(second )* 1000 +
			uint64(millisecond);
	}
}
