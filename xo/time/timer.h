#pragma once

#include "xo/system/xo_api.h"
#include "xo/time/time.h"

namespace xo
{
	struct XO_API timerv2
	{
		timerv2();
		time operator()() const;

		void reset();

	private:
		time epoch_;
	};
}
