#pragma once

#include "xo/xo_types.h"

namespace xo
{
	template< typename T >
	struct quat_
	{
	public:
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		constexpr quat_() : w( 1 ), x( 0 ), y( 0 ), z( 0 ) {}
		constexpr quat_( T w, T x, T y, T z ) : w( w ), x( x ), y( y ), z( z ) {}
		constexpr quat_( const quat_& o ) = default;

		/// conversion copy constructor
		template< typename U > explicit quat_( const quat_<U>& o ) : w( T( o.w ) ), x( T( o.x ) ), y( T( o.y ) ), z( T( o.z ) ) {}

		/// assignment
		quat_& operator=( const quat_& o ) = default;
		void set( T pw, T px, T py, T pz ) { w = pw; x = px ; y = py; z = pz; }

		/// member access
		T w, x, y, z;

		/// container methods
		size_t size() const { return 4; }
		iterator begin() { return &w; }
		iterator end() { return &w + 4; }
		const_iterator begin() const { return &w; }
		const_iterator end() const { return &w + 4; }

		/// generate quat with zero rotation
		static constexpr quat_<T> identity() { return quat_<T>( T(1), T(0), T(0), T(0) ); }
	};
		
	using quatf = quat_< float >;
	using quatd = quat_< double >;
}
