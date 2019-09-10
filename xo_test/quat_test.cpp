#include "xo/system/test_case.h"
#include "xo/geometry/quat.h"
#include "xo/geometry/vec3.h"

namespace xo
{
	XO_TEST_CASE( xo_quat_axes_test )
	{
		auto q = quat_from_euler( 20_deg, -30_deg, 40_deg );
		auto qx = local_x_axis( q );
		auto qy = local_y_axis( q );
		auto qz = local_z_axis( q );
		auto qx1 = q * vec3d::unit_x();
		auto qy1 = q * vec3d::unit_y();
		auto qz1 = q * vec3d::unit_z();
		auto qxyz = axes_from_quat( q );
		XO_CHECK( equal( qx, qx1 ) );
		XO_CHECK( equal( qy, qy1 ) );
		XO_CHECK( equal( qz, qz1 ) );
		XO_CHECK( qxyz.x == qx );
		XO_CHECK( qxyz.y == qy );
		XO_CHECK( qxyz.z == qz );
	}
}
