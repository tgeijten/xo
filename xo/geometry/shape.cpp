#include "shape.h"

#include "xo/container/prop_node_tools.h"
#include "xo/string/dictionary.h"

namespace xo
{
	xo::dictionary< xo::shape_type > shape_dict( {
		{ shape_type::sphere, "sphere" },
		{ shape_type::box, "box" },
		{ shape_type::capsule, "capsule" },
		{ shape_type::cylinder, "cylinder" },
		{ shape_type::cone, "cone" },
		{ shape_type::plane, "plane" }
	} );

	shape_type get_shape_type( const string& s )
	{
		return shape_dict( s );
	}

	shape::shape( const prop_node& pn ) :
	type_( get_shape_type( pn.get< string >( "type" ) ) ),
	x_(), y_(), z_()
	{
		switch ( type_ )
		{
		case shape_type::capsule:
		case shape_type::cylinder:
		case shape_type::cone:
			y_ = pn.get< float >( "height" );
			// fall through
		case shape_type::sphere:
			x_ = pn.get< float >( "radius" );
			break;
		case shape_type::box:
		{
			auto d = pn.get< vec3f >( "dim" );
			x_ = d.x, y_ = d.y, z_ = d.z;
			break;
		}
		case shape_type::plane:
		{
			auto n = pn.get< vec3f >( "normal" );
			x_ = n.x, y_ = n.y, z_ = n.z;
			break;
		}

		default: xo_error( "Unknown shape type" );
		}
	}

	shape::shape( shape_type t, float x_radius, float y_height, float z ) : type_( t ), x_( x_radius ), y_( y_height ), z_( z )
	{}

	const string& shape::name() const
	{
		return shape_dict( type_ );
	}

	float shape::radius() const
	{
		xo_assert( type_ == shape_type::sphere || type_ == shape_type::cylinder || type_ == shape_type::capsule || type_ == shape_type::cone );
		return x_;
	}

	float shape::height() const
	{
		xo_assert( type_ == shape_type::cylinder || type_ == shape_type::capsule || type_ == shape_type::cone );
		return y_;
	}

	float shape::half_height() const
	{
		xo_assert( type_ == shape_type::cylinder || type_ == shape_type::capsule || type_ == shape_type::cone );
		return 0.5f * y_;
	}

	vec3f shape::dim() const
	{
		switch ( type_ )
		{
		case shape_type::sphere: return vec3f( 2 * x_ );
		case shape_type::box: return vec3f( x_, y_, z_ );
		case shape_type::capsule:
		case shape_type::cylinder:
		case shape_type::cone:
			return vec3f( 2 * x_, y_, 2 * x_ );
		case shape_type::plane: return vec3f::zero();
		default: xo_error( "Cannot compute dim for " + name() );
		}
	}

	vec3f shape::half_dim() const
	{
		switch ( type_ )
		{
		case shape_type::sphere: return vec3f( x_ );
		case shape_type::box: return vec3f( 0.5f * x_, 0.5f * y_, 0.5f * z_ );
		case shape_type::capsule:
		case shape_type::cylinder:
		case shape_type::cone:
			return vec3f( x_, 0.5f * y_, x_ );
		default: xo_error( "Cannot compute half_dim for " + name() );
		}
	}

	float shape::volume() const
	{
		switch ( type_ )
		{
		case shape_type::sphere: return ( 4.0f / 3.0f ) * numconstf::pi() * cubed( radius() );
		case shape_type::box: return x_ * y_ * z_;
		case shape_type::cylinder: return numconstf::pi() * squared( radius() ) * height();
		default: xo_error( "Cannot compute volume for " + name() );
		}
	}

	vec3f shape::corner( index_t idx ) const
	{
		xo_assert( type_ == shape_type::box && idx < 8 );
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
		case shape_type::box:
		{
			bounding_boxf bb;
			for ( index_t i = 0; i < 8; ++i ) bb += t.p + t.q * corner( i );
			return bb;
		}
		case shape_type::sphere:
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
		// see: en.wikipedia.org/wiki/List_of_moments_of_inertia
		vec3f r;
		auto m = compute_mass( density );
		switch ( type_ )
		{
		case shape_type::sphere:
			r = vec3f( ( 2.0f / 5.0f ) * m * squared( radius() ) );
			break;
		case shape_type::box:
			r.x = m / 12.0f * ( y_ * y_ + z_ * z_ );
			r.y = m / 12.0f * ( x_ * x_ + z_ * z_ );
			r.z = m / 12.0f * ( x_ * x_ + y_ * y_ );
			break;

		case shape_type::cylinder:
			r.x = m / 12.0f * ( 3 * squared( radius() ) + squared( height() ) );
			r.y = r.x;
			r.z = 0.5f * m * squared( radius() ); // cylinder along z axis
			break;
		default: xo_error( "Cannot compute inertia for " + name() );
		}
		return r;
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
