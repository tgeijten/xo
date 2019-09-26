#pragma once

#include "color.h"
#include "xo/container/pair_type.h"
#include "xo/container/flat_map.h"
#include "xo/numerical/math.h"

#include <initializer_list>

namespace xo
{
	struct color_gradient
	{
		color_gradient( std::initializer_list< pair< float, color > > l ) : colors_( l ) {}

		color operator()( float v ) {
			// #todo: use generic interpolation algorithm
			auto up_it = colors_.upper_bound( v );
			if ( up_it == colors_.end() )
				return colors_.back().second;
			else if ( up_it == colors_.begin() )
				return colors_.front().second;
			else
			{
				auto lo_it = up_it;
				--lo_it;
				auto distance = up_it->first - lo_it->first;
				auto t = ( v - lo_it->first ) / distance;
				return xo::lerp( lo_it->second, up_it->second, t );
			}
		}

		flat_map< float, color > colors_;
	};
}
