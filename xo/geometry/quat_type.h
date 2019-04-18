#pragma once

#include "xo/xo_types.h"
#include "angle.h"
#include "xo/container/prop_node.h"

namespace xo
{
	template< typename T >
	struct quat_
	{
	public:
		quat_() : w( 1 ), x( 0 ), y( 0 ), z( 0 ) {}
		quat_( T w, T x, T y, T z ) : w( w ), x( x ), y( y ), z( z ) {}
		template< typename T2 > quat_( const quat_<T2>& o ) : w( T( o.w ) ), x( T( o.x ) ), y( T( o.y ) ), z( T( o.z ) ) {}

		/// assignment
		template< typename T2 > quat_<T>& operator=( const quat_<T2>& o ) { w = T( o.w ); x = T( o.x ); y = T( o.y ); z = T( o.z ); return *this; }
		template< typename T2 > void set( T2 pw, T2 px, T2 py, T2 pz ) { w = T( pw ); x = T( px ); y = T( py ); z = T( pz ); }

		/// conjugate
		quat_<T> conjugate() const { return quat_<T>( w, -x, -y, -z ); }

		/// inverse
		quat_<T> inverse() const { auto s = w * w + x * x + y * y + z * z; xo_assert( s > T( 0 ) ); auto f = T( -1 ) / s; return quat_<T>( w, f * x, f * y, f * z ); }

		/// member access
		T w, x, y, z;

		/// generate quat with zero rotation
		static quat_<T> identity() { return quat_<T>( T(1), T(0), T(0), T(0) ); }
	};
		
	using quatf = quat_< float >;
	using quatd = quat_< double >;

	template< typename T > prop_node to_prop_node( const quat_<T>& q ) {
		return prop_node().set( "w", q.w ).set( "x", q.x ).set( "y", q.y ).set( "z", q.z );
	}

	template< typename T > bool from_prop_node( const prop_node& pn, quat_<T>& q ) {
		if ( pn.size() == 4 )
		{
			q.w = pn.get<T>( "w" );
			q.x = pn.get<T>( "x" );
			q.y = pn.get<T>( "y" );
			q.z = pn.get<T>( "z" );
			return true;
		}
		else if ( vec3_< angle_< angle_unit::degrees, T > > v; from_prop_node( pn, v ) )
		{
			q = quat_from_euler( v );
			return true;
		}
		else return false;
	};
}
