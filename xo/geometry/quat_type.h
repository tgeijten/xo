#pragma once

#include "xo/utility/types.h"
#include "xo/container/prop_node.h"

namespace xo
{
	template< typename T >
	struct quat_
	{
	public:
		quat_() : w( 1 ), x( 0 ), y( 0 ), z( 0 ) {}
		quat_( const prop_node& pn ) : w( pn.get<T>( "w" ) ), x( pn.get<T>( "x" ) ), y( pn.get<T>( "y" ) ), z( pn.get<T>( "z" ) ) {}
		quat_( T w, T x, T y, T z ) : w( w ), x( x ), y( y ), z( z ) {}
		template< typename T2 > quat_( const quat_<T2>& o ) : w( T( o.w ) ), x( T( o.x ) ), y( T( o.y ) ), z( T( o.z ) ) {}

		/// assignment
		template< typename T2 > quat_<T>& operator=( const quat_<T2>& o ) { w = T( o.w ); x = T( o.x ); y = T( o.y ); z = T( o.z ); return *this; }
		template< typename T2 > void set( T2 pw, T2 px, T2 py, T2 pz ) { w = T( pw ); x = T( px ); y = T( py ); z = T( pz ); }

		/// convert to prop_node
		explicit operator prop_node() const { return prop_node().set( "w", w ).set( "x", x ).set( "y", y ).set( "z", z ); }

		/// conjugate
		quat_<T> conjugate() const { return quat_<T>( w, -x, -y, -z ); }

		/// inverse
		quat_<T> inverse() const { auto s = w * w + x * x + y * y + z * z; xo_assert( s > T( 0 ) ); auto f = T( -1 ) / s; return quat_<T>( w, f * x, f * y, f * z ); }

		/// member access
		T w, x, y, z;

		/// generate quat with zero rotation
		static quat_<T> identity() { return quat_<T>( T(1), T(0), T(0), T(0) ); }
	};
		
	typedef quat_< real_t > quat;
	typedef quat_< float > quatf;
	typedef quat_< double > quatd;

	template< typename T > struct prop_node_cast< quat_<T> > {
		static quat_<T> from( const prop_node& pn ) { return quat_<T>( pn ); }
		static prop_node to( const quat_<T>& q ) { return static_cast< prop_node >( q ); }
	};
}
