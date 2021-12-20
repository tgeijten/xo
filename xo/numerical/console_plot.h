#pragma once

#include <functional>
#include <algorithm>
#include <iostream>

namespace xo
{
	template< typename T >
	void console_plot( std::vector< std::function<T( T )> > fns, T x_min, T x_max, int width, int height ) {
		if ( width < 1 || height < 1 ) {
			std::cout << "width and height must be > 1" << std::endl;
			return;
		}
		if ( x_min >= x_max ) {
			std::cout << "x_min must be smaller than x_max" << std::endl;
			return;
		}


		std::vector< std::vector<T> > fn_ys;
		auto y_min = std::numeric_limits<T>::max();
		auto y_max = std::numeric_limits<T>::min();
		for ( auto& fn : fns ) {
			auto& fn_y = fn_ys.emplace_back();
			fn_y.reserve( width );
			for ( int i = 0; i < width; ++i ) {
				auto y = fn( x_min + i * ( x_max - x_min ) / width );
				fn_y.push_back( y );
				y_min = std::min( y_min, y );
				y_max = std::max( y_max, y );
			}
		}

		auto str = std::vector<std::string>( height, std::string( width, ' ' ) );
		auto line_idx = [&]( T yv ) {
			return height - 1 - static_cast<int>( std::round( ( height - 1 ) * ( yv - y_min ) / ( y_max - y_min ) ) ); };
		if ( y_min <= 0 && y_max >= 0 )
			str[ line_idx( 0 ) ] = std::string( width, char( 196 ) );

		for ( size_t fn_idx = 0; fn_idx < fns.size(); ++fn_idx ) {
			for ( int xi = 0; xi < width; ++xi ) {
				str[ line_idx( fn_ys[ fn_idx ][ xi ] ) ][ xi ] = char( 219 );
			}
		}

		for ( const auto& s : str )
			std::cout << s << std::endl;
	}
}
