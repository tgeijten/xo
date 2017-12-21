#pragma once

#include "xo/filesystem/path.h"
#include "xo/container/flat_map.h"
#include <functional>
#include "xo/filesystem/filesystem.h"

namespace xo
{
	template< typename T >
	struct file_resource_cache
	{
		file_resource_cache( std::function< T*( const path& ) > create_func ) : func_( create_func ) {}
		std::unique_ptr< T > operator()( const path& p )
		{
			auto timestamp = last_write_time( p );
			auto it = cache_.find( p );

			if ( it != cache_.end() && it->second.first < timestamp )
			{
				// update existing cache item
				log::trace( "Updating timestamp of ", p.filename(), " from ", it->second.first, " to ", timestamp );
				it->second.second = std::unique_ptr< T >( func_( p ) );
				it->second.first = timestamp;
			}
			else if ( it == cache_.end() )
			{
				// create new item and put it in the cache
				log::trace( "Creating new cached item for ", p.filename() );
				it = cache_.insert( std::make_pair( p, std::make_pair( timestamp, std::unique_ptr< T >( func_( p ) ) ) ) ).first;
			}

			// at this point, 'it' is a valid iterator, so we can return a copy
			log::trace( "Creating copy of ", p.filename() );
			return std::unique_ptr< T >( new T( *it->second.second ) );
		}

	private:
		std::function< T*( const path& ) > func_;
		flat_map< path, std::pair< time_t, std::unique_ptr< T > > > cache_;
	};
}
