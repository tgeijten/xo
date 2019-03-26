#pragma once

#include "xo/system/xo_api.h"
#include "xo/time/time.h"

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

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

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
