#pragma once

#include "xo/system/xo_api.h"
#include "xo/time/time.h"

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	namespace v2 {
		struct XO_API timer
		{
			timer();
			time operator()() const;
			void reset();

		private:
			long long epoch_;
		};
	}
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
