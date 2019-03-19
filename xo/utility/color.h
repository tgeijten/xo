#pragma once

#include "types.h"
#include "xo/container/prop_node.h"
#include "xo/system/platform.h"

namespace xo
{
	struct color
	{
		color() : r( 0.0f ), g( 0.0f ), b( 0.0f ), a( 1.0f ) {}
		color( float pr, float pg, float pb, float pa = 1.0f ) : r( pr ), g( pg ), b( pb ), a( pb ) {}
		color( double pr, double pg, double pb, double pa = 1.0 ) : r( float( pr ) ), g( float( pg ) ), b( float( pb ) ), a( float( pb ) ) {}
		color( const xo::prop_node& pn );

		float r, g, b, a;

		static color red( float br = 1.0f, float sat = 1.0f ) { return color( br, br*( 1.0f - sat ), br*( 1.0f - sat ) ); }
		static color yellow( float br = 1.0f, float sat = 1.0f ) { return color( br, br, br*( 1.0f - sat ) ); }
		static color green( float br = 1.0f, float sat = 1.0f ) { return color( br*( 1.0f - sat ), br, br*( 1.0f - sat ) ); }
		static color cyan( float br = 1.0f, float sat = 1.0f ) { return color( br*( 1.0f - sat ), br, br ); }
		static color blue( float br = 1.0f, float sat = 1.0f ) { return color( br*( 1.0f - sat ), br*( 1.0f - sat ), br ); }
		static color magenta( float br = 1.0f, float sat = 1.0f ) { return color( br, br*( 1.0f - sat ), br ); }
		static color white( float br = 1.0f ) { return color( br, br, br ); }
	};

	template< typename T > inline color operator*( T s, const color& c ) { return color( s * c.r, s * c.g, s * c.b, s * c.a ); }
	template< typename T > inline color operator*( const color& c, T s ) { return color( s * c.r, s * c.g, s * c.b, s * c.a ); }

	inline color operator+( const color& c1, const color& c2 ) { return color( c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a ); }
	inline color operator-( const color& c1, const color& c2 ) { return color( c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a ); }

	inline color mix_colors( const color& col1, const color& col2, float w1 ) { return w1 * col1 + ( 1.0f - w1 ) * col2; }

	XO_API float perceived_brightness( const color& c );
	XO_API color make_from_hsv( float H, float S, float V );
	XO_API color make_unique_color( index_t i, float brightness = 0.6 );
	XO_API color make_from_hex( unsigned int x );

	inline std::ostream& operator<<( std::ostream& str, const color c ) { return str << c.r << ' ' << c.g << ' ' << c.b << ' ' << c.a; }
	inline std::istream& operator>>( std::istream& str, color& c ) { return str >> c.r >> c.g >> c.b >> c.a; }

	inline color::color( const xo::prop_node& pn )
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
}

IS_PROP_NODE_CONSTRUCTABLE( xo::color );
