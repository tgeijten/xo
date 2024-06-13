#pragma once

#include "xo/xo_types.h"
#include "xo/geometry/vec3.h"
#include "xo/geometry/quat.h"
#include "xo/container/prop_node.h"
#include "scale3.h"

namespace xo
{
	template< typename T >
	struct transform_srt_
	{
		transform_srt_() : p(), q() {}
		transform_srt_( no_init_t ) : p( no_init ), q( no_init ) {}
		transform_srt_( vec3_<T> pos, quat_<T> ori ) : p( pos ), q( ori ) {}
		transform_srt_( vec3_<T> pos ) : p( pos ), q( quat_<T>::identity() ) {}
		transform_srt_( quat_<T> ori ) : p( vec3_<T>::zero() ), q( ori ) {}

		scale3_<T> s;
		quat_<T> q;
		vec3_<T> p;

		/// concatenate transforms
		transform_srt_<T> operator*( const transform_srt_<T>& t ) const { return transform_srt_<T>( p + q * t.p, q * t.q ); }
		transform_srt_<T>& operator*=( const transform_srt_<T>& t ) { p += q * t.p; q *= t.q; return *this; }
		transform_srt_<T> trans( const transform_srt_<T>& t ) const { return transform_srt_<T>( p + q * t.p, q * t.q ); }
		transform_srt_<T> inv() const { auto iq = -q; return transform_srt_<T>( iq * -p, iq ); }
		transform_srt_<T> inv_trans( const transform_srt_<T>& t ) const { return transform_srt_<T>( conjugate( q ) * ( t.p - p ), conjugate( q ) * t.q ); }

		vec3_<T> trans( const vec3_<T>& v ) const { return p + q * ( s * v ); }
		vec3_<T> operator*( const vec3_<T>& v ) const { return p + q * ( s * v ); }
		//vec3_<T> inv_trans( const vec3_<T>& v ) const { return conjugate( q ) * ( v - p ); }

		quat_<T> trans( const quat_<T>& o ) const { return q * o; }
		quat_<T> operator*( const quat_<T>& o ) const { return q * o; }
		//quat_<T> inv_trans( const quat_<T>& o ) const { return conjugate( q ) * o; }

		bool is_identity() const { return p.is_null() && q.is_identity() && s.is_one(); }
		static transform_srt_<T> identity() { return transform_srt_<T>( vec3_<T>::zero(), quat_<T>::identity() ); }
	};

	using transformf = transform_srt_< float >;
	using transformd = transform_srt_< double >;

	/// convert from prop_node
	template< typename T > bool from_prop_node( const prop_node& pn, transform_srt_<T>& tf ) {
		return from_prop_node( pn["p"], tf.p ) && from_prop_node( pn["q"], tf.q );
	}
}
