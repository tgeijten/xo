#pragma once

#include "xo/system/xo_api.h"
#include "xo/time/time.h"

namespace xo
{
	struct XO_API timer_v2
	{
		timer_v2();
		time operator()() const;

		void reset();

	private:
		time epoch_;
	};
}
