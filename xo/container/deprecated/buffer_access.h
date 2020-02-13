#pragma once

#include <vector>
#include <utility>
#include <algorithm>

namespace xo
{
	template< typename T > struct buffer_access
	{
		const static bool value = false; // SFINAE value
	};

	// specialization for std::vector< std::pair< key, value > >
	template< typename KeyT, typename ValueT >
	struct buffer_access< std::vector< std::pair< KeyT, ValueT > > >
	{
		typedef KeyT key_t;
		typedef ValueT value_t;
		typedef std::vector< std::pair< KeyT, ValueT > > buffer_t;
		typedef typename std::vector< std::pair< KeyT, ValueT > >::iterator iter_t;
		typedef typename std::vector< std::pair< KeyT, ValueT > >::const_iterator const_iter_t;
		static const_iter_t upper_bound( const buffer_t& buf, key_t key ) {
			return std::upper_bound( buf.cbegin(), buf.cend(), key,
				[&]( key_t lhs, const std::pair< key_t, value_t >& rhs ) { return lhs < rhs.first; } );
		}
		static const value_t& value_ref( const_iter_t iter ) { return iter->second; }
		const static bool value = true;
	};

	//// specialization for std::vector< value >
	//template< typename ValueT >
	//struct buffer_access< std::vector< ValueT > >
	//{
	//	typedef size_t key_t;
	//	typedef ValueT value_t;
	//	typedef std::vector< ValueT > buffer_t;
	//	typedef typename std::vector< ValueT >::iterator iter_t;
	//	typedef typename std::vector< ValueT >::const_iterator const_iter_t;
	//	static const_iter_t upper_bound( const buffer_t& buf, float key ) { return buf.begin() + size_t( key ); }
	//	static const value_t& value_ref( const_iter_t iter ) { return *iter; }
	//	const static bool value = true;
	//};
}
