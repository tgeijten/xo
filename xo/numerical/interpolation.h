#pragma once

#include <cmath>

namespace xo
{
	/// linearly interpolate between v1 (t=0) and v2 (t=1)
	template< typename T, typename S > T lerp( const T& v1, const T& v2, S t ) {
		return v1 + t * ( v2 - v1 );
	}

	template< typename M >
	typename M::mapped_type lerp_map( const M& cont, typename M::key_type key )
	{
		if ( cont.empty() )
			return typename M::mapped_type();
		auto ub = cont.upper_bound( key );
		if ( ub == cont.end() )
			return ( --ub )->second; // index is beyond data, return last sample
		else if ( ub == cont.begin() )
			return ub->second; // index is before data, return first sample
		else {
			auto lb = ub; lb--; // interpolate between two samples (lb and ub)
			auto t = ( key - lb->first ) / ( ub->first - lb->first );
			return lerp( lb->second, ub->second, t );
		}
	}

	template< typename It, typename K >
	auto lerp_span( It first, It last, K index ) {
		xo_assert( first < last );
		if ( index <= K( 0 ) )
			return *first;
		else {
			auto index_floor = std::floor( index );
			auto index_lb = static_cast<int>( index_floor );
			if ( index_lb + 1 >= last - first )
				return *( --last );
			first += index_lb;
			return lerp( *first, *( first + 1 ), index - index_floor );
		}
	}

	template< typename C, typename K >
	typename C::value_type lerp_vec( const C& cont, K index ) {
		return lerp_span( cont.begin(), cont.end(), index );
	}
}
