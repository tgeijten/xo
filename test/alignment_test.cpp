#include "xo/system/log.h"

namespace xo
{
	struct AA {
		int x;
	};

	struct AB {
		AA a1;
		AA a2;
	};

	struct AC {
		int a1;
		int a2;
	};

	void alignment_test()
	{
		set_global_log_level( log::trace_level );
		xo_logvar( sizeof( int ) );
		xo_logvar( sizeof( AA ) );
		xo_logvar( sizeof( AB ) );
		xo_logvar( sizeof( AC ) );
	}
}
