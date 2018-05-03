#pragma once

#include "xo/utility/types.h"
#include "xo/geometry/vec3.h"
#include "xo/geometry/quat.h"
#include "xo/container/prop_node.h"

namespace xo
{
	template< typename T >
	struct transform_
	{
		transform_() {}
		transform_( const prop_node& pn ) : p( pn[ "p" ] ), q( pn[ "q" ] ) {}
		transform_( vec3_<T> pos, quat_<T> ori ) : p( pos ), q( ori ) {}
		transform_( vec3_<T> pos ) : p( pos ), q( quat_<T>::identity() ) {}
		transform_( quat_<T> ori ) : p( vec3_<T>::zero() ), q( ori ) {}

		vec3_<T> p;
		quat_<T> q;

		/// convert to prop_node
		explicit operator prop_node() const { return prop_node().set( "p", p ).set( "q", q ); }

		/// concatenate transforms
		transform_<T> operator*( const transform_<T>& other ) const { return transform( other ); }
		transform_<T> operator-() const { return inverse(); }
		transform_<T> inverse() const { auto iq = -q; return transform_<T>( iq * -p, iq ); }

		vec3_<T> operator*( const vec3_<T>& v ) const { return p + q * v; }
		vec3_<T> transform( const vec3_<T>& v ) const { return p + q * v; }
		vec3_<T> inv_trans( const vec3_<T>& v ) const { return -q * ( v - p ); }

		quat_<T> operator*( const quat_<T>& o ) const { return q * o; }
		quat_<T> transform( const quat_<T>& o ) const { return q * o; }
		quat_<T> inv_trans( const quat_<T>& o ) const { return -q * o; }

		transform_<T> transform( const transform_<T>& t ) const { return transform_<T>( p + q * t.p, q * t.q ); }
		transform_<T> inv_trans( const transform_<T>& t ) const { return transform_<T>( -q * ( t.p - p ), -q * t.q ); }

		static transform_<T> identity() { return transform_<T>( vec3_<T>::zero(), quat_<T>::identity() ); }
	};

	typedef transform_< real_t > transform;
	typedef transform_< float > transformf;
	typedef transform_< double > transformd;

	template< typename T > struct prop_node_cast<transform_<T>> {
		static transform_<T> from( const prop_node& pn ) { return transform_<T>( pn ); }
		static prop_node to( const transform_<T>& vec ) { return static_cast<prop_node>( vec ); }
	};
}

