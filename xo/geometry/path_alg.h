#pragma once

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

	template< typename P >
	index_t insert_path_point( P& path, double insert_len )
	{
		auto s = std::size( path );
		xo_assert( s >= 2 );
		for ( index_t idx = 1; idx < s; ++idx )
		{
			if ( insert_len <= 0 )
				return idx;

			auto svec = path[ idx ] - path[ idx - 1 ];
			auto slen = length( svec );
			if ( insert_len < slen )
			{
				path.insert( path.begin() + idx, path[ idx - 1 ] + ( insert_len / slen ) * svec );
				return idx;
			}
			insert_len -= slen;
		}
		return no_index;
	}
}
