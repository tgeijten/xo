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
		transform_( vec3_<T> pos, quat_<T> ori ) : p( pos ), q( ori ) {}
		transform_( vec3_<T> pos ) : p( pos ), q( quat_<T>::identity() ) {}
		transform_( quat_<T> ori ) : p( vec3_<T>::zero() ), q( ori ) {}

		vec3_<T> p;
		quat_<T> q;

		/// convert to prop_node
		explicit operator prop_node() const { return prop_node().set( "p", p ).set( "q", q ); }

		/// concatenate transforms
		transform_<T> operator*( const transform_<T>& t ) const { return transform_<T>( p + q * t.p, q * t.q ); }
		transform_<T> inv() const { auto iq = -q; return transform_<T>( iq * -p, iq ); }
		transform_<T> inv_trans( const transform_<T>& t ) const { return transform_<T>( q.conjugate() * ( t.p - p ), q.conjugate() * t.q ); }

		vec3_<T> operator*( const vec3_<T>& v ) const { return p + q * v; }
		vec3_<T> inv_trans( const vec3_<T>& v ) const { return q.conjugate() * ( v - p ); }

		quat_<T> operator*( const quat_<T>& o ) const { return q * o; }
		quat_<T> inv_trans( const quat_<T>& o ) const { return q.conjugate() * o; }

		static transform_<T> identity() { return transform_<T>( vec3_<T>::zero(), quat_<T>::identity() ); }
	};

	using transformf = transform_< float >;
	using transformd = transform_< double >;
}

