#pragma once

#include "xo/xo_types.h"

namespace xo
{
	template< typename T >
	struct mat33_
	{
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		mat33_( no_init_t ) {}
		mat33_() : e00(), e01(), e02(), e10(), e11(), e12(), e20(), e21(), e22() {}
		mat33_( T v00, T v01, T v02, T v10, T v11, T v12, T v20, T v21, T v22 ) :
			e00( v00 ), e01( v01 ), e02( v02 ), e10( v10 ), e11( v11 ), e12( v12 ), e20( v20 ), e21( v21 ), e22( v22 ) {}
		~mat33_() {}

		T e00, e01, e02, e10, e11, e12, e20, e21, e22;

		T* operator[]( index_t row ) { return &ptr()[row * 3]; }
		const T* operator[]( index_t row ) const { return &ptr()[row * 3]; }
		T& operator()( index_t row, index_t col ) { return ptr()[row * 3 + col]; }
		const T& operator()( index_t row, index_t col ) const { return ptr()[row * 3 + col]; }

		T* ptr() { return &e00; }
		const T* ptr() const { return &e00; }

		static constexpr mat33_<T> zero() { return mat33_<T>(); }
		static constexpr mat33_<T> identity() { return mat33_<T>( T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1) ); }
	};

	using mat33f = mat33_<float>;
	using mat33d = mat33_<double>;
}
