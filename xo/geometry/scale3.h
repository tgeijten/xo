#pragma once

#include "xo/geometry/vec3_type.h"
#include "xo/container/prop_node.h"

namespace xo
{
	template< typename T > struct scale3_ : vec3_<T>
	{
		constexpr scale3_( no_init_t ) {}
		constexpr scale3_() : vec3_<T>( T( 1 ), T( 1 ), T( 1 ) ) {}
		constexpr scale3_( T v ) : vec3_<T>( v, v, v ) {}
		constexpr scale3_( const scale3_<T>& o ) = default;
		constexpr scale3_( const T& sx, const T& sy, const T& sz ) : vec3_<T>( sx, sy, sz ) {}
		constexpr scale3_( scale3_<T>&& o ) noexcept = default;

		// conversion constructors
		template< typename U > constexpr explicit scale3_( const U& sx, const U& sy, const U& sz ) : vec3_<T>( sx, sy, sz ) {}
		template< typename U > constexpr explicit scale3_( const scale3_<U>& o ) : vec3_<T>( o ) {} 

		/// assignment
		scale3_& operator=( const scale3_& o ) = default;
		scale3_& operator=( scale3_&& o ) noexcept = default;
		void set( const T& sx, const T& sy, const T& sz ) { this->x = sx; this->y = sy; this->z = sz; }

		/// const properties
		bool is_one() const { return this->x == T( 1 ) && this->y == T( 1 ) && this->z == T( 1 ); }

		/// set scale back to one
		void reset() { this->x = this->y = this->z = T( 1 ); }

		/// static initializers
		static constexpr vec3_<T> one() { return scale3_<T>( T( 1 ), T( 1 ), T( 1 ) ); }
		static constexpr vec3_<T> uniform( T v ) { return scale3_<T>( v, v, v ); }
	};

	/// template instantiations
	using scale3f = scale3_<float>;
	using scale3d = scale3_<double>;

	/// Scale operator *
	template< typename T > vec3_<T> operator*( const scale3_<T>& s, const vec3_<T>& v )
	{ return vec3_<T>( s.x * v.x, s.y * v.y, s.z * v.z ); }
	template< typename T > vec3_<T> operator*( const vec3_<T>& v, const scale3_<T>& s )
	{ return vec3_<T>( s.x * v.x, s.y * v.y, s.z * v.z ); }
	template< typename T > scale3_<T> operator*( const scale3_<T>& v, const scale3_<T>& s )
	{ return scale3_<T>( s.x * v.x, s.y * v.y, s.z * v.z ); }
	template< typename T > vec3_<T>& operator*=( vec3_<T>& v1, const scale3_<T>& v2 )
	{ v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }
	template< typename T > scale3_<T>& operator*=( scale3_<T>& v1, const scale3_<T>& v2 )
	{ v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }

	/// Inverse scale operator /
	template< typename T > vec3_<T> operator/( const vec3_<T>& v, const scale3_<T>& s )
	{ return vec3_<T>( v.x / s.x, v.y / s.y, v.z / s.z ); }
	template< typename T > scale3_<T> operator/( const scale3_<T>& v, const scale3_<T>& s )
	{ return scale3_<T>( v.x / s.x, v.y / s.y, v.z / s.z ); }
	template< typename T > vec3_<T>& operator/=( vec3_<T>& v1, const scale3_<T>& v2 )
	{ v1.x /= v2.x; v1.y /= v2.y; v1.z /= v2.z; return v1; }
	template< typename T > scale3_<T>& operator/=( scale3_<T>& v1, const scale3_<T>& v2 )
	{ v1.x /= v2.x; v1.y /= v2.y; v1.z /= v2.z; return v1; }

	/// convert from prop_node
	template< typename T > bool from_prop_node( const prop_node& pn, scale3_<T>& v ) {
		// load vec3_ from array [ 1 2 3 ], group { x=1 y=2 z=3 }, or scalar: "x"
		if ( pn.size() == 3 ) {
			if ( pn.is_array() ) // array
				v.set( pn.get<T>( 0 ), pn.get<T>( 1 ), pn.get<T>( 2 ) );
			else v.set( pn.get<T>( "x" ), pn.get<T>( "y" ), pn.get<T>( "z" ) ); // group
			return true;
		}
		else if ( pn.size() == 0 && pn.has_value() ) {
			// uniform scaling
			if ( auto sv = pn.try_get<T>() ) {
				v.set( *sv, *sv, *sv );
				return true;
			}
			else return false;
		}
		else return false;
	};
}
