#include "color.h"

#include <array>

#include "xo/system/log_sink.h"
#include "xo/numerical/math.h"
#include "xo/container/prop_node.h"

namespace xo
{
	bool from_prop_node( const prop_node& pn, color& c )
	{
		if ( pn.has_value() )
		{
			c = color_from_hex_rgb( static_cast<unsigned int>( std::stoul( pn.get<string>(), 0, 16 ) ) );
			return true;
		}
		else if ( pn.has_any_key( { "h", "s", "v" } ) )
		{
			c = color_from_hsv( pn.get<float>( "h", 0 ), pn.get<float>( "s", 1 ), pn.get<float>( "v", 1 ) );
			return true;
		}
		else if ( pn.size() >= 3 )
		{
			c.r = pn.get< float >( "r", pn.get<float>( 0 ) );
			c.g = pn.get< float >( "g", pn.get<float>( 1 ) );
			c.b = pn.get< float >( "b", pn.get<float>( 2 ) );
			return true;
		}
		else return false;
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
		float hue = standard_hue[ idx % standard_hue.size() ];
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

	uint32 hex_rgb_from_color( const color& c )
	{
		return float_to_byte( c.r ) << 16 | float_to_byte( c.g ) << 8 | float_to_byte( c.b );
	}

	float perceived_brightness( const color& c )
	{
		// see http://alienryderflex.com/hsp.html
		return sqrt( 0.299f * c.r * c.r + 0.587f * c.g * c.g + 0.114f * c.b * c.b );
	}
}
