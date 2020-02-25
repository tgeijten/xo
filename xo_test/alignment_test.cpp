#include "xo/system/log.h"

#include "xo/system/test_case.h"
#include "xo/xo_types.h"

namespace xo
{
	struct Empty {
	};

	struct AA {
		int32 x;
	};

	struct AB {
		AA a1;
		AA a2;
	};

	struct AC {
		int32 a1;
		int32 a2;
	};

	struct Test : Empty {
		int32 a1;
		int32 a2;
	};

	XO_TEST_CASE( xo_alignment_test )
	{
		XO_CHECK( sizeof( int32 ) == 4 );
		XO_CHECK( sizeof( AA ) == 4 );
		XO_CHECK( sizeof( AB ) == 8 );
		XO_CHECK( sizeof( AC ) == 8 );
		XO_CHECK( sizeof( Test ) == 8 );
	}
}
