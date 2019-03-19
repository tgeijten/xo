#pragma once

#include "color.h"

namespace vis
{
	struct color_gradient
	{
		color_gradient( std::initializer_list< std::pair< float, color > > l ) : colors_( l ) {}

		color operator()( float v ) {
			auto upper_it = colors_.upper_bound( v );
			if ( upper_it == colors_.end() )
				return colors_.back().second;
			else if ( upper_it == colors_.begin() )
				return colors_.front().second;
			else
			{
				auto lower_it = upper_it;
				--lower_it;

				auto d = upper_it->first - lower_it->first;
				auto w0 = 1.0f - ( v - lower_it->first ) / d;

				return w0 * lower_it->second + ( 1.0f - w0 ) * upper_it->second;
			}
		}

		xo::flat_map< float, color > colors_;
	};
}
