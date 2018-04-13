#include "shape.h"

#include "xo/container/prop_node_tools.h"

namespace xo
{
	shape::shape( const prop_node& pn )
	{
		INIT_PROP( pn, type_, box );
	}

	shape::shape( shape_type t, float x_radius, float y_height, float z ) : type_( t ), x_( x_radius ), y_( y_height ), z_( z )
	{}

	float shape::radius() const
	{
		xo_assert( type_ == sphere || type_ == cylinder || type_ == capsule || type_ == cone );
		return x_;
	}

	float shape::height() const
	{
		xo_assert( type_ == cylinder || type_ == capsule || type_ == cone );
		return y_;
	}

	float shape::half_height() const
	{
		xo_assert( type_ == cylinder || type_ == capsule || type_ == cone );
		return 0.5f * y_;
	}

	xo::vec3f shape::dim() const
	{
		switch ( type_ )
		{
		case xo::shape::sphere: return vec3f( 2 * x_ );
		case xo::shape::box: return vec3f( x_, y_, z_ );
		case xo::shape::capsule:
		case xo::shape::cylinder:
		case xo::shape::cone:
			return vec3f( 2 * x_, y_, 2 * x_ );
		default: xo_error( "Cannot compute dim for this shape" );
		}
	}

	xo::vec3f shape::half_dim() const
	{
		switch ( type_ )
		{
		case xo::shape::sphere: return vec3f( x_ );
		case xo::shape::box: return vec3f( 0.5f * x_, 0.5f * y_, 0.5f * z_ );
		case xo::shape::capsule:
		case xo::shape::cylinder:
		case xo::shape::cone:
			return vec3f( x_, 0.5f * y_, x_ );
		default: xo_error( "Cannot compute half_dim for this shape" );
		}
	}

	float shape::volume() const
	{
		switch ( type_ )
		{
		case box: return x_ * y_ * z_;
		case sphere: return ( 4.0f / 3.0f ) * pi<float>() * cubed( radius() );
		case cylinder: return pi<float>() * squared( radius() ) * height();
		default: xo_error( "Cannot compute volume for this shape" );
		}
	}

	xo::vec3f shape::corner( index_t idx ) const
	{
		xo_assert( type_ == box && idx < 8 );
		return vec3f(
			x_ * ( ( idx & 1 ) ? 0.5f : -0.5f ),
			y_ * ( ( idx & 2 ) ? 0.5f : -0.5f ),
			z_ * ( ( idx & 5 ) ? 0.5f : -0.5f )
		);
	}

	bounding_boxf shape::bounding_box( const transformf& t ) const
	{
		switch ( type_ )
		{
		case box:
		{
			bounding_boxf bb;
			for ( index_t i = 0; i < 8; ++i ) bb += t.p + t.q * corner( i );
			return bb;
		}
		case sphere: return bounding_boxf( t.p - vec3f( radius() ), t.p + vec3f( radius() ) );
		case capsule: xo_error( "Cannot compute bounding box for this shape" );
		default: xo_error( "Cannot compute bounding box for this shape" );
		}
	}

	void shape::scale( float factor )
	{
		x_ *= factor;
		y_ *= factor;
		z_ *= factor;
	}

	void shape::scale( const vec3f& factor )
	{
		x_ *= factor.x;
		y_ *= factor.y;
		z_ *= factor.z;
	}
}
