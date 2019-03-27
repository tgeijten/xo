#pragma once

#include "xo/system/xo_api.h"
#include "xo/time/time.h"

namespace xo
{
	struct XO_API timer
	{
		timer();
		time operator()() const;
		void reset();

	private:
		long long epoch_;
	};
}
