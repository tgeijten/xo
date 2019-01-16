#pragma once

#include <array>

namespace xo
{
	template< typename T, int N >
	struct iir_filter
	{
		iir_filter() : x_(), y_(), b0_(), b_(), a_() {}
		const T& operator()( const T& x ) {
			auto y = b0_ * x;
			for ( int i = 0; i < N; ++i )
				y += b_[ i ] * x_[ i ] + a_[ i ] * y_[ i ];

			// shift values, TODO: use index instead for better performance
			for ( int i = N - 1; i > 0; --i ) {
				x_[ i ] = x_[ i - 1 ];
				y_[ i ] = y_[ i - 1 ];
			}

			x_[ 0 ] = x;
			y_[ 0 ] = y;
			return y_[ 0 ];
		}
		const T& operator()() const { return y_[ 0 ]; }
		T velocity() const { return y_[ 0 ] - y_[ 1 ]; }

		T b0_;
		std::array< T, N > x_;
		std::array< T, N > b_;
		std::array< T, N > y_;
		std::array< T, N > a_;
	};

	template< typename T > iir_filter< T, 2 > make_lowpass_butterworth_2nd_order( T cutoff_ratio )
	{
		// https://stackoverflow.com/questions/20924868/calculate-coefficients-of-2nd-order-butterworth-low-pass-filter
		iir_filter< T, 2 > f;
		const T ita = T(1) / std::tan( constants<T>::pi() * cutoff_ratio );
		const T q = sqrt( T(2) );
		f.b0_ = T(1) / ( T(1) + q * ita + ita * ita );
		f.b_[ 0 ] = T(2) * f.b0_;
		f.b_[ 1 ] = f.b0_;
		f.a_[ 0 ] = T(2) * ( ita * ita - T(1) ) * f.b0_;
		f.a_[ 1 ] = -( T(1) - q * ita + ita * ita ) * f.b0_;
		return f;
	}
}
