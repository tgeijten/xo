#include "timer.h"
#include "xo/system/assert.h"

namespace xo
{
	timerv2::timerv2() :
	epoch_( 0 )
	{

	}

	time timerv2::operator()() const
	{
		XO_NOT_IMPLEMENTED;
	}

	void timerv2::reset()
	{

	}
}
