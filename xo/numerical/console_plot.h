#pragma once

#include <functional>
#include <algorithm>
#include <iostream>
#include <array>

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

		auto x_span = x_max - x_min;
		auto y_min = std::numeric_limits<T>::max();
		auto y_max = std::numeric_limits<T>::min();

		std::vector< std::vector<T> > fn_ys;
		for ( auto& fn : fns ) {
			auto& fn_y = fn_ys.emplace_back();
			fn_y.reserve( width );
			for ( int xi = 0; xi < width; ++xi ) {
				auto y = fn( x_min + xi * x_span / ( width - 1 ) );
				fn_y.push_back( y );
				y_min = std::min( y_min, y );
				y_max = std::max( y_max, y );
			}
		}
		auto y_span = y_max - y_min;

		auto grid_x = [&]( T x ) {
			return static_cast<int>( std::floor( 0.5 + ( width - 1 ) * ( x - x_min ) / x_span ) ); };
		auto grid_y = [&]( T y ) {
			return height - 1 - static_cast<int>( std::floor( 0.5 + ( height - 1 ) * ( y - y_min ) / y_span ) ); };

		auto str = std::vector<std::string>( height, std::string( width, ' ' ) );

		int yi0 = grid_y( 0 );
		str[ 0 ] = std::string( width, char( 196 ) );
		str[ height - 1 ] = std::string( width, char( 196 ) );
		if ( yi0 >= 0 && yi0 < height )
			str[ yi0 ] = std::string( width, char( 196 ) );

		int xi0 = grid_x( 0 );
		if ( x_min <= 0 && x_max >= 0 )
			for ( int yi = 0; yi < height; ++yi )
				str[ yi ][ xi0 ] = char( yi == 0 ? 194 : yi == yi0 ? 197 : yi == ( height - 1 ) ? 193 : 179 );

		static std::array plot_chars = { char( 219 ), char( 178 ), char( 177 ), char( 176 ) };
		for ( int fni = 0; fni < fns.size(); ++fni ) {
			for ( int xi = 0; xi < width; ++xi ) {
				str[ grid_y( fn_ys[ fni ][ xi ] ) ][ xi ] = plot_chars[ fni % plot_chars.size() ];
			}
		}

		for ( const auto& s : str )
			std::cout << s << std::endl;
	}
}
