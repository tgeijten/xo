#pragma once

#include "xo/geometry/euler_angles.h"
#include "xo/numerical/random.h"

namespace xo
{
	std::pair< quatf, vec3radf > make_random_quat( euler_order eu = euler_order::xyz, random_number_generator& rng = global_random_number_generator() ) {
		bounds b( -180.0f, 180.0f );
		vec3radf rot( degreef( rng.uni( b ) ), degreef( rng.uni( b ) ), degreef( rng.uni( b ) ) );
		return { quat_from_euler( rot, eu ), rot };
	}
}

