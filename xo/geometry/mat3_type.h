#pragma once

#include "xo/xo_types.h"

namespace xo
{
	template< typename T >
	struct mat3_
	{
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		mat3_( no_init_t ) {}
		mat3_() : e00(), e01(), e02(), e10(), e11(), e12(), e20(), e21(), e22() {}
		mat3_( T v00, T v01, T v02, T v10, T v11, T v12, T v20, T v21, T v22 ) :
			e00( v00 ), e01( v01 ), e02( v02 ), e10( v10 ), e11( v11 ), e12( v12 ), e20( v20 ), e21( v21 ), e22( v22 ) {}
		~mat3_() {}

		T e00, e01, e02, e10, e11, e12, e20, e21, e22;

		T* operator[]( index_t row ) { return &data()[row * 3]; }
		const T* operator[]( index_t row ) const { return &data()[row * 3]; }
		T& operator()( index_t row, index_t col ) { return data()[row * 3 + col]; }
		const T& operator()( index_t row, index_t col ) const { return data()[row * 3 + col]; }

		T* data() { return &e00; }
		const T* data() const { return &e00; }

		static constexpr mat3_<T> zero() { return mat3_<T>(); }
		static constexpr mat3_<T> identity() { return mat3_<T>( T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1) ); }
	};

	using mat3f = mat3_<float>;
	using mat3d = mat3_<double>;
}
