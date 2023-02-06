#include <vector>
#include <map>

#include "xo/geometry/vec3.h"
#include "xo/system/log.h"
#include "xo/system/test_case.h"
#include "xo/container/container_algorithms.h"
#include "xo/container/flat_map.h"
#include "xo/numerical/interpolation.h"

namespace xo
{
	XO_TEST_CASE( xo_interpolation_test )
	{
		// map interpolation
		std::map< float, vec3f > m;
		m[0.0f] = vec3f( 1, 0, 0 );
		m[1.0f] = vec3f( 0, 1, 0 );
		m[3.0f] = vec3f( 0, 1, 2 );
		XO_CHECK( equal( lerp_map( m, -1.0f ), vec3f( 1, 0, 0 ) ) );
		XO_CHECK( equal( lerp_map( m, 0.0f ), vec3f( 1, 0, 0 ) ) );
		XO_CHECK( equal( lerp_map( m, 0.1f ), vec3f( 0.9f, 0.1f, 0.0f ) ) );
		XO_CHECK( equal( lerp_map( m, 0.8f ), vec3f( 0.2f, 0.8f, 0.0f ) ) );
		XO_CHECK( equal( lerp_map( m, 1.0f ), vec3f( 0, 1, 0 ) ) );
		XO_CHECK( equal( lerp_map( m, 2.0f ), vec3f( 0, 1, 1 ) ) );
		XO_CHECK( equal( lerp_map( m, 100.0f ), vec3f( 0, 1, 2 ) ) );

		// more tests
		std::vector< vec3f > vec;
		vec.push_back( vec3f( 1, 0, 0 ) );
		vec.push_back( vec3f( 0, 1, 0 ) );
		vec.push_back( vec3f( 0, 1, 2 ) );
		XO_CHECK( equal( lerp_vec( vec, -1.0f ), vec3f( 1, 0, 0 ) ) );
		XO_CHECK( equal( lerp_vec( vec, 0.0f ), vec3f( 1, 0, 0 ) ) );
		XO_CHECK( equal( lerp_vec( vec, 0.1f ), vec3f( 0.9f, 0.1f, 0.0f ) ) );
		XO_CHECK( equal( lerp_vec( vec, 0.8f ), vec3f( 0.2f, 0.8f, 0.0f ) ) );
		XO_CHECK( equal( lerp_vec( vec, 1.5f ), vec3f( 0, 1, 1 ) ) );
		XO_CHECK( equal( lerp_vec( vec, 2.0f ), vec3f( 0, 1, 2 ) ) );
		XO_CHECK( equal( lerp_vec( vec, 2.1f ), vec3f( 0, 1, 2 ) ) );
		XO_CHECK( equal( lerp_vec( vec, 100.0f ), vec3f( 0, 1, 2 ) ) );

		flat_map<double, double> fm;
		fm[-1.0] = 9.0;
		fm[0.0] = 10.0;
		fm[3.0] = 40.0;
		XO_CHECK( equal( lerp_map( fm, -10 ), 9.0 ) );
		XO_CHECK( equal( lerp_map( fm, -0.5 ), 9.5 ) );
		XO_CHECK( equal( lerp_map( fm, 0 ), 10.0 ) );
		XO_CHECK( equal( lerp_map( fm, 0.1 ), 11.0 ) );
		XO_CHECK( equal( lerp_map( fm, 2.9 ), 39.0 ) );
		XO_CHECK( equal( lerp_map( fm, 100.0 ), 40.0 ) );
	}
}
