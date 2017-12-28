#pragma once

#include "xo/utility/types.h"

namespace xo
{
	template< typename T >
	struct mat33_
	{
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		mat33_() {}
		mat33_( T v00, T v01, T v02, T v10, T v11, T v12, T v20, T v21, T v22 ) :
			e00( v00 ), e01( v01 ), e02( v02 ), e10( v10 ), e11( v11 ), e12( v12 ), e20( v20 ), e21( v21 ), e22( v22 ) {}
		~mat33_() {}

		T* operator[]( index_t row ) { return &ptr()[ row * 3 ]; }
		T* ptr() { return &e00; }
		T e00, e01, e02, e10, e11, e12, e20, e21, e22;
	};

	typedef mat33_< real_t > mat33;
	typedef mat33_< float > mat33f;
	typedef mat33_< double > mat33d;
}
