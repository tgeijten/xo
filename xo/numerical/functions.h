#pragma once

#include "constants.h"

namespace xo
{
	// fast implementation of arccos, approx 4x faster than standard math.h
	// https://stackoverflow.com/questions/3380628/fast-arc-cos-algorithm/3380723#3380723
	template< typename T > T fast_acos( T x ) {
		constexpr T a = T( -0.939115566365855 );
		constexpr T b = T( 0.9217841528914573 );
		constexpr T c = T( -1.2845906244690837 );
		constexpr T d = T( 0.295624144969963174 );
		T num = a * x;
		T x2 = x * x;
		T den = 1 + c * x2;
		T x3 = x2 * x;
		num += b * x3;
		den += d * x3 * x;
		return constants<T>::half_pi() + num / den;
	}

	// minimum jerk curve for x in [0, 1]
	template< typename T > T minimum_jerk( T x ) {
		auto x3 = x * x * x;
		auto x4 = x3 * x;
		auto x5 = x4 * x;
		return 6 * x5 - 15 * x4 + 10 * x3;
	}
}
