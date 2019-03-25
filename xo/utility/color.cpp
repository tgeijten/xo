#include "color.h"

#include <array>

#include "xo/system/log_sink.h"
#include "xo/numerical/math.h"

namespace xo
{
	color::color( const xo::prop_node& pn )
	{
		if ( pn.has_value() )
			*this = make_from_hex( static_cast<unsigned int>( std::stoul( pn.get_value(), 0, 16 ) ) );
		else if ( pn.has_any_key( { "h", "s", "v" } ) )
			*this = make_from_hsv( pn.get<float>( "h", 0 ), pn.get<float>( "s", 1 ), pn.get<float>( "v", 1 ) );
		else if ( pn.size() >= 3 )
		{
			if ( auto rr = pn.try_get< float >( "r" ) ) r = *rr; else r = pn.get<float>( 0 );
			if ( auto gg = pn.try_get< float >( "g" ) ) g = *gg; else g = pn.get<float>( 1 );
			if ( auto bb = pn.try_get< float >( "b" ) ) b = *bb; else b = pn.get<float>( 2 );
		}
		else xo_error( "Could not read color from prop_node" );
	}

	color make_from_hsv( float H, float S, float V )
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
		float pbr = perceived_brightness( make_from_hsv( hue, sat, 1.0f ) );
		float val = xo::clamped( brightness / pbr, brightness, 1.0f );
		return make_from_hsv( hue, sat, val );
	}

	color make_from_hex( unsigned int x )
	{
		return color( ( x >> 16 ) / 255.0f, ( ( x & 0xff00 ) >> 8 ) / 255.0f, ( x & 0xff ) / 255.0f );
	}

	float perceived_brightness( const color& c )
	{
		// see http://alienryderflex.com/hsp.html
		return sqrt( 0.299f * c.r * c.r + 0.587f * c.g * c.g + 0.114f * c.b * c.b );
	}
}
