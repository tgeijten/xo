#include "xo/system/test_case.h"
#include "xo/time/time.h"
#include <limits>
#include "xo/system/log.h"

namespace xo
{
	XO_TEST_CASE( xo_time_test )
	{
		xo::time t1( 15625000 );
		xo::time t2( 15625001 );

		auto s1f = t1.secondsf();
		auto s1d = t1.secondsd();
		auto s2f = t2.secondsf();
		auto s2d = t2.secondsd();

		auto t1f = time_from_seconds( s1f );
		auto t1d = time_from_seconds( s1d );
		auto t2f = time_from_seconds( s2f );
		auto t2d = time_from_seconds( s2d );

		XO_CHECK( t1 == t1f );
		XO_CHECK( t1 == t1d );
		XO_CHECK( t2 != t2f );
		XO_CHECK( t2 == t2d );
	}
}
