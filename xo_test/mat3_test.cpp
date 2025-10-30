#include "xo/system/test_case.h"
#include "xo/geometry/mat33.h"
#include "xo/numerical/random.h"
#include "xo/geometry/angle_type.h"
#include "xo/geometry/quat.h"
#include "xo/numerical/constants.h"

namespace xo
{
	XO_TEST_CASE( mat33_test )
	{
		auto eps = 2 * num<double>::epsilon;
		random_number_generator_fast rng;
		for ( int i = 0; i < 20; ++i ) {
			auto q = rng.uniform_quat( -180_degd, 180_degd );
			auto v = rng.uniform_vec3( -1.0, 1.0 );
			auto m = mat3_from_quat( q );
			auto vq = q * v;
			auto vm = m * v;
			XO_CHECK_IF_EQUAL_EPSILON_ABS( vq, vm, eps );

			auto qm = quat_from_mat3( m );
			XO_CHECK_IF_EQUAL_EPSILON_ABS( canonical( q ), canonical( qm ), eps );

			auto q2 = rng.uniform_quat( -180_degd, 180_degd );
			auto m2 = mat3_from_quat( q2 );
			auto qq2 = q * q2;
			auto mm2 = m * m2;
			auto qmm2 = quat_from_mat3( mm2 );
			XO_CHECK_IF_EQUAL_EPSILON_ABS( canonical( qq2 ), canonical( qmm2 ), eps );
		}
	}
}
