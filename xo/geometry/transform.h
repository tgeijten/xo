#pragma once

#include "xo/xo_types.h"
#include "xo/geometry/vec3.h"
#include "xo/geometry/quat.h"
#include "xo/container/prop_node.h"

namespace xo
{
	template< typename T >
	struct transform_
	{
		transform_() : p(), q() {}
		transform_( no_init_t ) : p( no_init ), q( no_init ) {}
		transform_( vec3_<T> pos, quat_<T> ori ) : p( pos ), q( ori ) {}
		transform_( vec3_<T> pos ) : p( pos ), q( quat_<T>::identity() ) {}
		transform_( quat_<T> ori ) : p( vec3_<T>::zero() ), q( ori ) {}

		vec3_<T> p;
		quat_<T> q;

		/// convert to prop_node
		explicit operator prop_node() const { return prop_node().set( "p", p ).set( "q", q ); }

		/// concatenate transforms
		transform_<T> operator*( const transform_<T>& t ) const { return transform_<T>( p + q * t.p, q * t.q ); }
		transform_<T>& operator*=( const transform_<T>& t ) { p += q * t.p; q *= t.q; return *this; }
		transform_<T> trans( const transform_<T>& t ) const { return transform_<T>( p + q * t.p, q * t.q ); }
		transform_<T> inv() const { auto iq = -q; return transform_<T>( iq * -p, iq ); }
		transform_<T> inv_trans( const transform_<T>& t ) const { return transform_<T>( conjugate( q ) * ( t.p - p ), conjugate( q ) * t.q ); }

		vec3_<T> operator*( const vec3_<T>& v ) const { return p + q * v; }
		vec3_<T> trans( const vec3_<T>& v ) const { return p + q * v; }
		vec3_<T> inv_trans( const vec3_<T>& v ) const { return conjugate( q ) * ( v - p ); }

		quat_<T> operator*( const quat_<T>& o ) const { return q * o; }
		quat_<T> trans( const quat_<T>& o ) const { return q * o; }
		quat_<T> inv_trans( const quat_<T>& o ) const { return conjugate( q ) * o; }

		bool is_identity() const { return p.is_null() && q.is_identity(); }
		static transform_<T> identity() { return transform_<T>( vec3_<T>::zero(), quat_<T>::identity() ); }
	};

	using transformf = transform_< float >;
	using transformd = transform_< double >;

	/// convert from prop_node
	template< typename T > bool from_prop_node( const prop_node& pn, transform_<T>& tf ) {
		return from_prop_node( pn["p"], tf.p ) && from_prop_node( pn["q"], tf.q );
	}
}
