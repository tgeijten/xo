#pragma once

#include "xo/xo_types.h"
#include <utility>

namespace xo
{
	template< typename T > struct vec3_
	{
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		constexpr vec3_() : x(), y(), z() {}
		constexpr vec3_( const vec3_<T>& o ) = default;
		constexpr vec3_( const T& px, const T& py, const T& pz ) : x( px ), y( py ), z( pz ) {}
		constexpr vec3_( vec3_<T>&& o ) noexcept = default;
		constexpr vec3_( T&& px, T&& py, T&& pz ) : x( std::move( px ) ), y( std::move( py ) ), z( std::move( pz ) ) {}

		// conversion constructors
		template< typename U > explicit vec3_( const U& px, const U& py, const U& pz ) : x( T( px ) ), y( T( py ) ), z( T( pz ) ) {}
		template< typename U > explicit vec3_( const vec3_<U>& o ) : x( T( o.x ) ), y( T( o.y ) ), z( T( o.z ) ) {}

		/// assignment
		vec3_& operator=( const vec3_& o ) = default;
		vec3_& operator=( vec3_&& o ) noexcept = default;
		void set( const T& px, const T& py, const T& pz ) { x = px; y = py; z = pz; }

		/// data
		T x, y, z;

		/// element access
		const T& operator[]( index_t idx ) const { return (&x)[idx]; }
		T& operator[]( index_t idx ) { return (&x)[idx]; }

		/// const properties
		bool is_null() const { return x == T(0) && y == T(0) && z == T(0); }

		/// modify vec3
		void clear() { x = y = z = T(0); }

		/// container methods
		size_t size() const { return 3; }
		iterator begin() { return &x; }
		iterator end() { return &x + 3; }
		const_iterator begin() const { return &x; }
		const_iterator end() const { return &x + 3; }

		/// static initializers
		static constexpr vec3_<T> zero() { return vec3_<T>( T(0), T(0), T(0) ); }
		static constexpr vec3_<T> unit_x() { return vec3_<T>( T(1), T(0), T(0) ); }
		static constexpr vec3_<T> unit_y() { return vec3_<T>( T(0), T(1), T(0) ); }
		static constexpr vec3_<T> unit_z() { return vec3_<T>( T(0), T(0), T(1) ); }
		static constexpr vec3_<T> neg_unit_x() { return vec3_<T>( T(-1), T(0), T(0) ); }
		static constexpr vec3_<T> neg_unit_y() { return vec3_<T>( T(0), T(-1), T(0) ); }
		static constexpr vec3_<T> neg_unit_z() { return vec3_<T>( T(0), T(0), T(-1) ); }
		static constexpr vec3_<T> diagonal( T v ) { return vec3_<T>( v, v, v ); }
		static constexpr vec3_<T> axis( index_t idx ) { return vec3_<T>( T( idx == 0 ), T( idx == 1 ), T( idx == 2 ) ); }
	};

	/// template instantiations
	using vec3f = vec3_< float >;
	using vec3d = vec3_< double >;
}
