#pragma once

#include "xo/xo_types.h"
#include "xo/container/prop_node.h" // TODO: get rid of this header
#include "xo/string/string_cast.h"
#include "xo/string/string_tools.h"

namespace xo
{
	template< typename T > struct vec3_
	{
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		vec3_() : x(), y(), z() {}
		vec3_( T px, T py, T pz ) : x( px ), y( py ), z( pz ) {}
		vec3_( T v ) : x( v ), y( v ), z( v ) {}
		template< typename T2 > vec3_( const vec3_<T2>& o ) : x( T( o.x ) ), y( T( o.y ) ), z( T( o.z ) ) {}

		/// assignment
		template< typename T2 > vec3_<T>& operator=( const vec3_<T2>& o ) { x = T( o.x ); y = T( o.y ); z = T( o.z ); return *this; }
		template< typename T2 > void set( T2 px, T2 py, T2 pz ) { x = T( px ); y = T( py ); z = T( pz ); }

		/// element access
		const T& operator[]( index_t idx ) const { return (&x)[idx]; }
		T& operator[]( index_t idx ) { return (&x)[idx]; }
		iterator begin() { return &x; }
		iterator end() { return &x + 3; }
		const_iterator begin() const { return &x; }
		const_iterator end() const { return &x + 3; }
		T x, y, z;

		/// const properties
		T length() const { return sqrt( x * x + y * y + z * z ); }
		T squared_length() const { return x * x + y * y + z * z; }
		bool is_null() const { return x == T(0) && y == T(0) && z == T(0); }
		void clear() { x = y = z = T(0); }

		/// container methods
		size_t size() const { return 3; }

		/// static initializers
		static vec3_<T> zero() { return vec3_<T>( T(0), T(0), T(0) ); }
		static vec3_<T> unit_x() { return vec3_<T>( T(1), T(0), T(0) ); }
		static vec3_<T> unit_y() { return vec3_<T>( T(0), T(1), T(0) ); }
		static vec3_<T> unit_z() { return vec3_<T>( T(0), T(0), T(1) ); }
		static vec3_<T> neg_unit_x() { return vec3_<T>( T(-1), T(0), T(0) ); }
		static vec3_<T> neg_unit_y() { return vec3_<T>( T(0), T(-1), T(0) ); }
		static vec3_<T> neg_unit_z() { return vec3_<T>( T(0), T(0), T(-1) ); }
		static vec3_<T> uniform( T v ) { return vec3_<T>( v, v, v ); }
		static vec3_<T> axis( index_t idx ) { return vec3_<T>( T( idx == 0 ), T( idx == 1 ), T( idx == 2 ) ); }
	};

	/// template instantiations
	using vec3f = vec3_< float >;
	using vec3d = vec3_< double >;

	// export float and double instances
	template struct XO_API vec3_< float >;
	template struct XO_API vec3_< double >;

	template< typename T > string to_str( const vec3_<T>& v ) { return to_str( v.x ) + ' ' + to_str( v.y ) + ' ' + to_str( v.z ); }
	template< typename T > bool from_str( const string& s, vec3_<T>& v ) {
		if ( auto vs = split_str( s, " \t" ); vs.size() >= 3 )
			return from_str( vs[ 0 ], v.x ) && from_str( vs[ 1 ], v.y ) && from_str( vs[ 2 ], v.z );
		else return false;
	}

	// TODO: move this to a different file?
	template< typename T > bool from_prop_node( const prop_node& pn, vec3_<T>& v ) {
		v.x = pn.get( "x", T() );
		v.y = pn.get( "y", T() );
		v.z = pn.get( "z", T() );
		if ( pn.size() >= 3 && pn.is_array() ) {
			v.x = pn.get<T>( 0 );
			v.y = pn.get<T>( 1 );
			v.z = pn.get<T>( 2 );
			return true;
		}
		else if ( pn.size() == 0 && pn.has_value() ) {
			return from_str( pn.get_value(), v );
		}
		else return false;
	};

	/// convert to prop_node
	template< typename T > prop_node to_prop_node( const vec3_<T>& v ) {
		return prop_node().set( "x", v.x ).set( "y", v.y ).set( "z", v.z );
	}
}
