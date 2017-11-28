#pragma once

#include "xo/geometry/vec3_type.h"
#include <utility>
#include <vector>
#include "xo/container/buffer_access.h"
#include "xo/system/assert.h"
// TODO not on UNIX #include <xtr1common>

namespace xo
{
	template< typename T >
	typename buffer_access<T>::value_t interpolated_value( const T& cont, typename buffer_access<T>::key_t key )
	{
		xo_assert( !cont.empty() );

		auto ub = buffer_access<T>::upper_bound( cont, key );
		if ( ub == cont.cend() )
			return buffer_access<T>::value_ref( --ub ); // key is beyond data, return last sample
		else if ( ub == cont.cbegin() )
			return buffer_access<T>::value_ref( ub ); // key is before data, return first sample
		else
		{
			// interpolate between two samples (lb and ub)
			auto lb = ub - 1;
			typename buffer_access<T>::key_t uw = ( key - lb->first ) / ( ub->first - lb->first );
			return ( typename buffer_access<T>::key_t(1) - uw ) * buffer_access<T>::value_ref( lb ) + uw * buffer_access<T>::value_ref( ub );
		}
	}

	template< typename T, typename K >
	T interpolated_value( const std::vector< T >& cont, K key, typename std::enable_if< !buffer_access< std::vector< T > >::value >::type* = 0 ) 
	{
		xo_assert( !cont.empty() );

		if ( key < K(0) )
			return cont.front();
		else
		{
			auto lb = size_t( key );
			auto ub = lb + 1;
			if ( ub >= cont.size() )
				return cont.back();

			K w = key - lb;
			return ( K(1) - w ) * cont[ lb ] + w * cont[ ub ];
		}
	}
}
