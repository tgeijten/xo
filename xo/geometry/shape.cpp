#include "shape.h"

#include "xo/container/prop_node_tools.h"
#include "xo/string/dictionary.h"

namespace xo
{
	xo::dictionary< xo::shape::shape_type > shape::shape_dict( { { sphere, "sphere" },{ box, "box" },{ capsule, "capsule" },{ cylinder, "cylinder" },{ cone, "cone" } } );

	shape::shape( const prop_node& pn )
	{
		INIT_PROP( pn, type_, box );
	}

	shape::shape( shape_type t, float x_radius, float y_height, float z ) : type_( t ), x_( x_radius ), y_( y_height ), z_( z )
	{}

	const string& shape::name() const
	{
		return shape_dict( type_ );
	}

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

	vec3f shape::dim() const
	{
		switch ( type_ )
		{
		case shape::sphere: return vec3f( 2 * x_ );
		case shape::box: return vec3f( x_, y_, z_ );
		case shape::capsule:
		case shape::cylinder:
		case shape::cone:
			return vec3f( 2 * x_, y_, 2 * x_ );
		default: xo_error( "Cannot compute dim for " + name() );
		}
	}

	vec3f shape::half_dim() const
	{
		switch ( type_ )
		{
		case shape::sphere: return vec3f( x_ );
		case shape::box: return vec3f( 0.5f * x_, 0.5f * y_, 0.5f * z_ );
		case shape::capsule:
		case shape::cylinder:
		case shape::cone:
			return vec3f( x_, 0.5f * y_, x_ );
		default: xo_error( "Cannot compute half_dim for " + name() );
		}
	}

	float shape::volume() const
	{
		switch ( type_ )
		{
		case sphere: return ( 4.0f / 3.0f ) * pi<float>() * cubed( radius() );
		case box: return x_ * y_ * z_;
		case cylinder: return pi<float>() * squared( radius() ) * height();
		default: xo_error( "Cannot compute volume for " + name() );
		}
	}

	vec3f shape::corner( index_t idx ) const
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
		case sphere:
			return bounding_boxf( t.p - vec3f( radius() ), t.p + vec3f( radius() ) );
		default: xo_error( "Cannot compute bounding box for " + name() );
		}
	}

	float shape::compute_mass( float density ) const
	{
		return density * volume();
	}

	vec3f shape::compute_inertia( float density ) const
	{
		if ( density == 0.0f )
			return vec3f::zero();

		auto mass = compute_mass( density );

		switch ( type_ )
		{
		case sphere:
			// see: en.wikipedia.org/wiki/List_of_moments_of_inertia
			return vec3f( ( 2.0f / 5.0f ) * mass * squared( radius() ) );
		case cylinder:
		{
			// see: en.wikipedia.org/wiki/List_of_moments_of_inertia
			auto hi = ( 1.0f / 12.0f ) * mass * ( 3 * squared( radius() ) + squared( height() ) );
			auto vi = 0.5f * mass * squared( radius() );
			return vec3f( hi, hi, vi ); // cylinder along z axis
		}
		default: xo_error( "Cannot compute inertia for " + name() );
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
