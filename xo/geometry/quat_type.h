#pragma once

#include "xo/xo_types.h"

namespace xo
{
	template< typename T >
	struct quat_
	{
	public:
		constexpr quat_() : w( 1 ), x( 0 ), y( 0 ), z( 0 ) {}
		constexpr quat_( T w, T x, T y, T z ) : w( w ), x( x ), y( y ), z( z ) {}
		template< typename T2 > quat_( const quat_<T2>& o ) : w( T( o.w ) ), x( T( o.x ) ), y( T( o.y ) ), z( T( o.z ) ) {}

		/// assignment
		template< typename T2 > quat_<T>& operator=( const quat_<T2>& o ) { w = T( o.w ); x = T( o.x ); y = T( o.y ); z = T( o.z ); return *this; }
		template< typename T2 > void set( T2 pw, T2 px, T2 py, T2 pz ) { w = T( pw ); x = T( px ); y = T( py ); z = T( pz ); }

		/// const properties
		T length() const { return sqrt( w * w + x * x + y * y + z * z ); }
		T squared_length() const { return w * w + x * x + y * y + z * z; }
		quat_<T> conjugate() const { return quat_<T>( w, -x, -y, -z ); }
		quat_<T> inverse() const { auto s = w * w + x * x + y * y + z * z; xo_assert( s > T( 0 ) ); auto f = T( -1 ) / s; return quat_<T>( w, f * x, f * y, f * z ); }

		/// member access
		T w, x, y, z;

		/// generate quat with zero rotation
		static constexpr quat_<T> identity() { return quat_<T>( T(1), T(0), T(0), T(0) ); }
	};
		
	using quatf = quat_< float >;
	using quatd = quat_< double >;
}
