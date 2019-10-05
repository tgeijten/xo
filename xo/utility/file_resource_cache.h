#pragma once

#include "xo/container/flat_map.h"
#include "xo/filesystem/filesystem.h"
#include "xo/utility/pointer_types.h"
#include "xo/container/container_tools.h"
#include "xo/string/string_type.h"

#include <functional>
#include <mutex>
#include <ctime>

namespace xo
{
	template< typename T, typename PathT >
	struct file_resource_cache
	{
		std::unique_ptr< T > operator()( const PathT& p )
		{
			std::lock_guard< std::mutex > lock( mutex_ );

			// make sure file exists
			xo_error_if( !file_exists( p ), "File does not exist: " + to_str( p ) );

			// find file in cache
			auto it = cache_.find( p );
			if ( it != cache_.end() )
			{
				auto wall_time = std::time( nullptr );
				if ( std::difftime( wall_time, it->second.last_checked_time_ ) > min_check_modified_interval )
				{
					try
					{
						// check if the file has been modified
						it->second.last_checked_time_ = wall_time;
						auto file_time = last_write_time( p );
						if ( file_time != it->second.file_time_ )
						{
							// file was modified, reload
							it->second.file_time_ = file_time;
							it->second.data_ = std::make_unique<T>( p );
						}
					}
					catch( std::exception& e ) {
						xo_error( "Error updating resource: "s + e.what() );
					}
				}
			}
			else
			{				
				try
				{
					// file not found in cache, create new item and put it in the cache
					it = cache_.insert( std::make_pair( p, cached_item() ) ).first;
					it->second.file_time_ = last_write_time( p );
					it->second.last_checked_time_ = std::time( nullptr );
					it->second.data_ = std::make_unique<T>( p );
				}
				catch ( std::exception& e ) {
					xo_error( "Error creating resource: "s + e.what() );
				}
			}

			// at this point, 'it' is a valid iterator, so we can return a copy
			return std::unique_ptr< T >( new T( *it->second.data_ ) );
		}

	private:
		static constexpr double min_check_modified_interval = 1.0; // interval in seconds

		std::mutex mutex_;
		struct cached_item {
			time_t file_time_;
			time_t last_checked_time_;
			u_ptr<T> data_;
		};
		flat_map< PathT, cached_item > cache_;
	};
}
