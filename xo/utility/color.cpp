#include "color.h"

#include <array>
#include <string>

#include "xo/system/log_sink.h"
#include "xo/numerical/math.h"
#include "xo/container/prop_node.h"
#include "xo/string/string_tools.h"

namespace xo
{
	bool from_prop_node( const prop_node& pn, color& c )
	{
		try {
			if ( pn.has_value() ) {
				auto str = xo::to_lower( pn.get_str() );
				bool has_rgb = xo::str_begins_with( str, "rgb" );
				bool has_hsv = xo::str_begins_with( str, "hsv" );
				if ( has_rgb || has_hsv ) {
					auto v = xo::split_str( str, ",; ()" );
					if ( v.size() == 4 ) {
						if ( has_rgb ) {
							c = color{ std::stof( v[1] ) / 255, std::stof( v[2] ) / 255, std::stof( v[3] ) / 255 };
							return true;
						}
						else if ( has_hsv ) {
							c = color_from_hsv( std::stof( v[1] ), std::stof( v[2] ) / 100, std::stof( v[3] ) / 100 );
							return true;
						}
						else return false;
					}
					else return false;
				}
				else {
					bool has_0x = xo::str_begins_with( str, "0x" );
					size_t num_digits = has_0x ? str.size() - 2 : str.size();
					auto value = static_cast<uint32>( std::stoul( str, 0, 16 ) );

					if ( num_digits == 8 )
						c = color_from_hex_rgba( value );
					else c = color_from_hex_rgb( value );
					return true;
				}
			}
			else if ( pn.has_any_key( { "h", "s", "v" } ) ) {
				c = color_from_hsv( pn.get<float>( "h", 0 ), pn.get<float>( "s", 1 ), pn.get<float>( "v", 1 ) );
				return true;
			}
			else if ( pn.size() >= 3 ) {
				c.r = pn.get< float >( "r", pn.get<float>( 0 ) );
				c.g = pn.get< float >( "g", pn.get<float>( 1 ) );
				c.b = pn.get< float >( "b", pn.get<float>( 2 ) );
				if ( pn.size() >= 4 )
					c.a = pn.get< float >( "a", pn.get<float>( 3 ) );
				return true;
			}
			else return false;
		}
		catch ( std::exception& ) { return false; }
	}

	color color_from_hsv( float H, float S, float V )
	{
		H = fmod( H, 360.0f ) / 60.0f;
		float fract = H - floor( H );
		float P = V * ( 1.0f - S );
		float Q = V * ( 1.0f - S * fract );
		float T = V * ( 1.0f - S * ( 1.0f - fract ) );

		if ( 0. <= H && H < 1. )
			return color( V, T, P );
		else if ( 1. <= H && H < 2. )
			return color( Q, V, P );
		else if ( 2. <= H && H < 3. )
			return color( P, V, T );
		else if ( 3. <= H && H < 4. )
			return color( P, Q, V );
		else if ( 4. <= H && H < 5. )
			return color( T, P, V );
		else if ( 5. <= H && H < 6. )
			return color( V, P, Q );
		else
			return color( 0., 0., 0. );
	}

	color make_unique_color( index_t idx, float brightness )
	{
		static std::array< float, 12 > standard_hue{ 0, 120, 240, 45, 195, 310, 20, 75, 165, 215, 275, 330 };
		float hue = standard_hue[idx % standard_hue.size()];
		float sat = 1.0f / ( 1.0f + idx / standard_hue.size() );
		float pbr = perceived_brightness( color_from_hsv( hue, sat, 1.0f ) );
		float val = xo::clamped( brightness / pbr, brightness, 1.0f );
		return color_from_hsv( hue, sat, val );
	}

	color color_from_hex_rgb( uint32 x )
	{
		const float f = 1.0f / 255.0f;
		return color( f * get_byte( x, 16 ), f * get_byte( x, 8 ), f * get_byte( x, 0 ) );
	}

	color color_from_hex_rgba( uint32 x )
	{
		const float f = 1.0f / 255.0f;
		return color( f * get_byte( x, 24 ), f * get_byte( x, 16 ), f * get_byte( x, 8 ), f * get_byte( x, 0 ) );
	}

	uint32 hex_rgb_from_color( const color& c )
	{
		return float_to_byte( c.r ) << 16 | float_to_byte( c.g ) << 8 | float_to_byte( c.b );
	}

	uint32 hex_rgba_from_color( const color& c )
	{
		return float_to_byte( c.r ) << 24 | float_to_byte( c.g ) << 16 | float_to_byte( c.b ) << 8 | float_to_byte( c.a );
	}

	bool operator<( const color& c1, const color& c2 )
	{
		if ( c1.r == c2.r ) {
			if ( c1.g == c2.g ) {
				if ( c1.b == c2.b ) {
					return c1.a < c2.a;
				}
				else return c1.b < c2.b;
			}
			else return c1.g < c2.g;
		}
		else return c1.r < c2.r;
	}

	float perceived_brightness( const color& c )
	{
		// see http://alienryderflex.com/hsp.html
		return std::sqrt( 0.299f * c.r * c.r + 0.587f * c.g * c.g + 0.114f * c.b * c.b );
	}
}
