#pragma once

#include "xo/geometry/quat.h"
#include "xo/numerical/random.h"
#include "xo/container/pair_type.h"

namespace xo
{
	template< typename T, typename E >
	pair< quat_<T>, vec3rad_<T> > make_random_quat( euler_order eu, random_number_generator_<E>& rng ) {
		bounds<T> b( -180.0f, 180.0f );
		vec3degf rot( degreef( rng.uniform( b ) ), degreef( rng.uniform( b ) ), degreef( rng.uniform( b ) ) );
		return { quat_from_euler( rot, eu ), vec3radf( rot ) };
	}

	template< typename T, typename E > vec3_<T> make_random_vec3( const bounds<T>& b, random_number_generator_<E>& rng ) {
		return vec3_<T>( rng.uniform( b ), rng.uniform( b ), rng.uniform( b ) );
	}
}
