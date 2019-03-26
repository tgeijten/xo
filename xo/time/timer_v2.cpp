#include "timer_v2.h"
#include "xo/system/assert.h"

namespace xo
{
	timer_v2::timer_v2() :
	epoch_( 0 )
	{

	}

	time timer_v2::operator()() const
	{
		XO_NOT_IMPLEMENTED;
	}

	void timer_v2::reset()
	{

	}
}
