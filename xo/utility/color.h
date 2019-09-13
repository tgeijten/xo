#pragma once

#include "xo/xo_types.h"

namespace xo
{
	struct XO_API color
	{
		color() : r( 0.0f ), g( 0.0f ), b( 0.0f ), a( 1.0f ) {}
		color( float pr, float pg, float pb, float pa = 1.0f ) : r( pr ), g( pg ), b( pb ), a( pb ) {}
		color( double pr, double pg, double pb, double pa = 1.0 ) : r( float( pr ) ), g( float( pg ) ), b( float( pb ) ), a( float( pb ) ) {}

		float r, g, b, a;

		static color red( float br = 1.0f, float sat = 1.0f ) { return color( br, br * ( 1.0f - sat ), br * ( 1.0f - sat ) ); }
		static color orange( float br = 1.0f, float sat = 1.0f ) { return color( br, 0.5f * br + 0.5f * br * ( 1.0f - sat ), br * ( 1.0f - sat ) ); }
		static color yellow( float br = 1.0f, float sat = 1.0f ) { return color( br, br, br * ( 1.0f - sat ) ); }
		static color green( float br = 1.0f, float sat = 1.0f ) { return color( br * ( 1.0f - sat ), br, br * ( 1.0f - sat ) ); }
		static color cyan( float br = 1.0f, float sat = 1.0f ) { return color( br * ( 1.0f - sat ), br, br ); }
		static color blue( float br = 1.0f, float sat = 1.0f ) { return color( br * ( 1.0f - sat ), br * ( 1.0f - sat ), br ); }
		static color magenta( float br = 1.0f, float sat = 1.0f ) { return color( br, br * ( 1.0f - sat ), br ); }
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
	XO_API bool from_prop_node( const prop_node& pn, color& c );
}
