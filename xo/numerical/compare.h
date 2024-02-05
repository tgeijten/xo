#pragma once

#include "constants.h"
#include "math.h"

namespace xo
{
	/// compare floating point variables using ample_epsilon
	/// IMPORTANT: this may not work as expected with values close to zero
	template< typename T > bool equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return abs( v1 - v2 ) <= abs( v1 ) * e; }

	/// compare floating point variables using ample_epsilon
	template< typename T > bool less_than_or_equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return v1 < v2 || equal( v1, v2, e ); }

	/// compare floating point variables using ample_epsilon
	template< typename T > bool less_than_not_equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return v1 < v2 && !equal( v1, v2, e ); }

	/// compare floating point variables using ample_epsilon
	template< typename T > bool greater_than_or_equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return v1 > v2 || equal( v1, v2, e ); }

	/// compare floating point variables using ample_epsilon
	template< typename T > bool greater_than_not_equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return v1 > v2 && !equal( v1, v2, e ); }
}
