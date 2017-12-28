#pragma once

#include <cmath>

#include "xo/utility/types.h"

namespace xo
{
	template< typename T > struct vec4_
	{
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		vec4_() : x(), y(), z(), w() {}
		vec4_( T px, T py, T pz, T pw ) : x( px ), y( py ), z( pz ), w( pw ) {}

		// element access
		const T& operator[]( index_t idx ) const { return (&x)[idx]; }
		T& operator[]( index_t idx ) { return (&x)[idx]; }
		T x, y, z, w;

		// const properties
		T length() const { return sqrt( x * x + y * y + z * z + w * w ); }
		T squared_length() const { return x * x + y * y + z * z + w * w; }
		bool is_null() const { return x == T() && y == T() && z == T() && w == T(); }

		// static initializers
		static vec4_<T> zero() { return vec4_<T>( T(), T(), T(), T() ); }
		static vec4_<T> unit_x() { return vec4_<T>( T(1), T(), T(), T() ); }
		static vec4_<T> unit_y() { return vec4_<T>( T(), T(1), T(), T() ); }
		static vec4_<T> unit_z() { return vec4_<T>( T(), T(), T(1), T() ); }
		static vec4_<T> unit_w() { return vec4_<T>( T(), T(), T(), T(1) ); }
	};

	/// template instantiations
	typedef vec4_< real_t > vec4;
	typedef vec4_< float > vec4f;
	typedef vec4_< double > vec4d;
}
