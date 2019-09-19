#pragma once

#include "xo/geometry/quat.h"
#include "xo/numerical/random.h"
#include "xo/container/pair_type.h"

namespace xo
{
	template< typename T >
	pair< quat_<T>, vec3rad_<T> > make_random_quat( euler_order eu = euler_order::xyz, random_number_generator& rng = global_random_number_generator() ) {
		bounds<T> b( -180.0f, 180.0f );
		vec3degf rot( degreef( rng.uni( b ) ), degreef( rng.uni( b ) ), degreef( rng.uni( b ) ) );
		return { quat_from_euler( rot, eu ), vec3radf( rot ) };
	}

	template< typename T > vec3_<T> make_random_vec3( const bounds<T>& b, random_number_generator& rng = global_random_number_generator() ) {
		return vec3_<T>( rng.uni( b ), rng.uni( b ), rng.uni( b ) );
	}
}
