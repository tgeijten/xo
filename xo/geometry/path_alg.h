#pragma once

#include "xo/numerical/compare.h"

namespace xo
{
	template< typename P >
	double path_length( const P& path )
	{
		auto b = std::begin( path );
		auto e = std::end( path );
		xo_assert( b != e );
		double len = 0;
		for ( auto b2 = b + 1; b2 != e; ++b, ++b2 )
			len += length( *b2 - *b );
		return len;
	}

	template< typename P, typename T >
	index_t insert_path_point( P& path, T insert_len )
	{
		auto s = std::size( path );
		xo_assert( s >= 2 );

		// check if this matches the first existing point
		if ( less_or_equal( insert_len, T( 0 ) ) )
			return 0;
			
		for ( index_t idx = 1; idx < s; ++idx )
		{
			auto svec = path[ idx ] - path[ idx - 1 ];
			auto slen = length( svec );

			if ( equal( insert_len, slen ) )
			{
				// return existing point
				return idx;
			}
			else if ( insert_len < slen )
			{
				// new point inside this segment
				path.insert( path.begin() + idx, path[ idx - 1 ] + ( insert_len / slen ) * svec );
				return idx;
			}
			insert_len -= slen;
		}

		return s - 1; // return last point
	}
}
