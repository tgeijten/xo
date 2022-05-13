#include "xo/system/test_case.h"
#include "xo/geometry/vec3.h"
#include "test_tools.h"
#include "xo/geometry/dynvec.h"

namespace xo
{
	XO_TEST_CASE( dyn_vec_test )
	{
		dynvecd v( 3 );
		vec3d v3{ 1, 2, 3 };

		XO_CHECK( v.is_null() );
		v[ 0 ] = 1; v[ 1 ] = 2; v[ 2 ] = 3;

		XO_CHECK( equal( v.length(), length( v3 ) ) );
		normalize( v );
		normalize( v3 );

		XO_CHECK( equal( v[ 0 ], v3[ 0 ] ) );
		XO_CHECK( equal( v[ 1 ], v3[ 1 ] ) );
		XO_CHECK( equal( v[ 2 ], v3[ 2 ] ) );
	}
}
