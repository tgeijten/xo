#pragma once

#include "xo/geometry/vec3_type.h"

namespace xo
{
	template< typename T > struct scale3_ : vec3_<T>
	{
		constexpr scale3_( no_init_t ) {}
		constexpr scale3_() : x( T( 1 ) ), y( T( 1 ) ), z( T( 1 ) ) {}
		constexpr scale3_( const scale3_<T>& o ) = default;
		constexpr scale3_( const T& px, const T& py, const T& pz ) : x( px ), y( py ), z( pz ) {}
		constexpr scale3_( scale3_<T>&& o ) noexcept = default;

		// conversion constructors
		template< typename U > constexpr explicit scale3_( const U& px, const U& py, const U& pz ) : x( T( px ) ), y( T( py ) ), z( T( pz ) ) {}
		template< typename U > constexpr explicit scale3_( const scale3_<U>& o ) : x( T( o.x ) ), y( T( o.y ) ), z( T( o.z ) ) {}

		/// assignment
		scale3_& operator=( const scale3_& o ) = default;
		scale3_& operator=( scale3_&& o ) noexcept = default;
		void set( const T& px, const T& py, const T& pz ) { x = px; y = py; z = pz; }

		/// const properties
		bool is_one() const { return x == T( 1 ) && y == T( 1 ) && z == T( 1 ); }

		/// set scale back to one
		void reset() { x = y = z = T( 1 ); }
	};

	/// template instantiations
	using scale3f = scale3_<float>;
	using scale3d = scale3_<double>;

	/// Scale vector elements
	template< typename T > vec3_<T> operator*( const scale3_<T>& s, const vec3_<T>& v )
	{ return vec3_<T>( s.x * v.x, s.y * v.y, s.z * v.z ); }
	template< typename T > vec3_<T> operator*( const vec3_<T>& v, const scale3_<T>& s )
	{ return vec3_<T>( s.x * v.x, s.y * v.y, s.z * v.z ); }

	/// Inverse scale vector elements
	template< typename T > vec3_<T> operator/( const vec3_<T>& v, const scale3_<T>& s )
	{ return vec3_<T>( v.x / s.x, v.y / s.y, v.z / s.z ); }
}
