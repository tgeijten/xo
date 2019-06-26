#pragma once

#include "xo/container/prop_node.h"
#include "xo/geometry/vec3_type.h"
#include "xo/geometry/angle.h"
#include "xo/geometry/quat_type.h"
#include "xo/numerical/bounds.h"
#include "xo/geometry/euler_angles.h"
#include "xo/string/string_tools.h"

namespace xo
{
	template< typename T > prop_node to_prop_node( const quat_<T>& q ) {
		prop_node pn;
		pn.set( "w", q.w );
		pn.set( "x", q.x );
		pn.set( "y", q.y );
		pn.set( "z", q.z );
		return pn;
	}

	template< typename T > bool from_prop_node( const prop_node& pn, quat_<T>& q ) {
		if ( pn.size() == 4 && pn.has_key( "w" ) ) { // literal quaterion with w, x, y, z
			q.set( pn.get<T>( "w" ), pn.get<T>( "x" ), pn.get<T>( "y" ), pn.get<T>( "z" ) );
			return true;
		}
		else if ( vec3_< degree_<T> > v; from_prop_node( pn, v ) ) { // euler angles
			auto order = pn.get< euler_order >( "order", euler_order::xyz );
			q = quat_from_euler( v, order );
			return true;
		}
		else return false;
	};

	template< typename T > bool from_prop_node( const prop_node& pn, transform_<T>& tf ) {
		return from_prop_node( pn[ "p" ], tf.p ) && from_prop_node( pn[ "q" ], tf.q );
	}

	template< typename T > bool from_prop_node( const prop_node& pn, vec3_<T>& v ) {
		v.set( pn.get( "x", T() ), pn.get( "y", T() ), pn.get( "z", T() ) );
		if ( pn.size() >= 3 && pn.is_array() ) {
			v.set( pn.get<T>( 0 ), pn.get<T>( 1 ), pn.get<T>( 2 ) );
		}
		else if ( pn.size() == 0 && pn.has_value() ) {
			if ( auto vs = split_str( pn.raw_value(), " \t" ); vs.size() >= 3 )
				return from_str( vs[ 0 ], v.x ) && from_str( vs[ 1 ], v.y ) && from_str( vs[ 2 ], v.z );
			else return false;
		}
		return true;
	};

	template< typename T > prop_node to_prop_node( const vec3_<T>& v ) {
		prop_node pn;
		pn.set( "x", v.x );
		pn.set( "y", v.y );
		pn.set( "z", v.z );
		return pn;
	}

	template< typename T > bool from_prop_node( const prop_node& pn, bounds<T>& v ) {
		if ( pn.has_value() ) {
			return from_str( pn.raw_value(), v );
		}
		else if ( pn.size() >= 2 ) {
			v.lower = pn.get_any<T>( { "min", "lower" }, pn.get<T>( 0 ) );
			v.upper = pn.get_any<T>( { "max", "upper" }, pn.get<T>( 1 ) );
			return true;
		}
		else return false;
	}
}
