#pragma once

#include "xo/container/table.h"
#include "xo/container/storage.h"
#include <vector>

namespace xo
{
	template< typename T, typename L >
	std::vector< T > compute_channel_means( const storage< T, L >& sto )
	{
		// compute means
		std::vector< T > means( sto.channel_size() );
		for ( index_t row = 0; row < sto.frame_size(); ++row )
			for ( index_t col = 0; col < sto.channel_size(); ++col )
				means[col] += sto( row, col );
		for ( auto& m : means )
			m /= sto.frame_size();

		return means;
	}

	template< typename T, typename L >
	table< T > compute_covariance( const storage< T, L >& sto )
	{
		auto nchannels = sto.channel_size();
		auto nframes = sto.frame_size();

		auto means = compute_channel_means( sto );
		table< T, L > cv( nchannels, nchannels );
		for ( size_t row = 0; row < nchannels; ++row )
		{
			for ( size_t col = 0; col < nchannels; ++col )
			{
				T v = T( 0 );
				for ( index_t i = 0; i < sto.frame_size(); ++i )
					v += ( sto( i, row ) - means[row] ) * ( sto( i, col ) - means[col] );
				cv( row, col ) = v / nframes;
			}
		}
		return cv;
	}
}
