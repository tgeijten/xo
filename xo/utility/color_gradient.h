#pragma once

#include "color.h"
#include "xo/container/pair_type.h"
#include "xo/container/flat_map.h"
#include "xo/numerical/interpolation.h"

#include <initializer_list>

namespace xo
{
	struct color_gradient
	{
		color_gradient( std::initializer_list< pair< float, color > > l ) : colors_( l ) {}
		color operator()( float v ) {
			return lerp_map( colors_, v );
		}
		flat_map< float, color > colors_;
	};
}
