#pragma once

#include "xo/filesystem/path.h"
#include "xo/container/flat_map.h"
#include "xo/filesystem/filesystem.h"

#include <functional>
#include <mutex>

namespace xo
{
	template< typename T >
	struct file_resource_cache
	{
		file_resource_cache( std::function< T*( const path& ) > create_func ) : func_( create_func ) {}
		std::unique_ptr< T > operator()( const path& p )
		{
			std::lock_guard< std::mutex > lock( mutex_ );

			// find file in cache
			auto timestamp = last_write_time( p );
			auto it = cache_.find( p );
			if ( it != cache_.end() && it->second.first != timestamp )
			{
				// file found in cache with different timestamp
				it->second.second = std::unique_ptr< T >( func_( p ) );
				it->second.first = timestamp;
			}
			else if ( it == cache_.end() )
			{				
				// file not found in cache, create new item and put it in the cache
				it = cache_.insert( std::make_pair( p, std::make_pair( timestamp, std::unique_ptr< T >( func_( p ) ) ) ) ).first;
			}

			// at this point, 'it' is a valid iterator, so we can return a copy
			return std::unique_ptr< T >( new T( *it->second.second ) );
		}

	private:
		std::mutex mutex_;
		std::function< T*( const path& ) > func_;
		flat_map< path, std::pair< time_t, std::unique_ptr< T > > > cache_;
	};
}
