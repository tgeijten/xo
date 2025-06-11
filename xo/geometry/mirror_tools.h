#pragma once

#include "vec3_type.h"
#include "quat_type.h"
#include "transform.h"
#include "xo/numerical/bounds.h"
#include "xo/utility/side.h"
#include "xo/string/pattern_matcher.h"

namespace xo
{
	inline void mirror( double& v ) { v = -v; }
	inline void mirror( float& v ) { v = -v; }
	inline void mirror( int& v ) { v = -v; }

	template< typename T >
	void mirror( vec3_<T>& pos ) {
		pos.z = -pos.z;
	}

	template< typename T >
	void mirror_axis( vec3_<T>& axis ) {
		axis.x = -axis.x;
		axis.y = -axis.y;
	}

	template< typename T >
	void mirror( quat_<T>& ori ) {
		ori.x = -ori.x;
		ori.y = -ori.y;
	}

	template< typename T >
	void mirror( bounds<T>& b ) {
		auto upper = -b.lower;
		b.lower = -b.upper;
		b.upper = upper;
	}

	template< typename T >
	void mirror( transform_<T>& tf ) {
		mirror( tf.p );
		mirror( tf.q );
	}

	template< typename T >
	void mirror( vec3_<bounds<T>>& limits ) {
		mirror( limits.x );
		mirror( limits.y );
	}

	inline void mirror( string& name ) {
		str_mirror_side( name );
	}

	inline void mirror( pattern_matcher& pm ) {
		for ( auto& p : pm.patterns() )
			mirror( p );
	}

	template< typename T >
	T mirrored( T obj ) {
		mirror( obj );
		return obj;
	}

	template< typename T >
	T& mirror_if( T& obj, bool m ) {
		if ( m )
			mirror( obj );
		return obj;
	}

	template< typename T >
	T& mirror_axis_if( vec3_<T>& obj, bool m ) {
		if ( m )
			mirror_axis( obj );
		return obj;
	}

	template< typename T >
	T mirrored_if( T obj, bool m ) {
		mirror_if( obj, m );
		return obj;
	}

	template< typename T >
	vec3_<T> mirrored_axis_if( vec3_<T> dir, bool m ) {
		mirror_axis_if( obj, m );
		return obj;
	}
}
