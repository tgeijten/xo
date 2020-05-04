#pragma once

#include "xo/filesystem/filesystem.h"
#include "xo/utility/pointer_types.h"
#include "xo/container/container_tools.h"
#include "xo/string/string_type.h"

#include <mutex>
#include <ctime>
#include <map>

namespace xo
{
	template< typename T, typename PathT >
	struct file_resource_cache
	{
		std::unique_ptr< T > operator()( const PathT& p )
		{
			std::lock_guard< std::mutex > lock( mutex_ );

			auto it = cached_items_.find( p );
			cached_item* item = it != cached_items_.end() ? &it->second : nullptr;

			if ( item )
			{
				// check if file was modified
				auto wall_time = std::time( nullptr );
				if ( std::difftime( wall_time, item->last_checked_time_ ) > min_check_modified_interval )
				{
					try
					{
						// check if the file has been modified
						item->last_checked_time_ = wall_time;
						auto file_time = last_write_time( p );
						if ( file_time != item->file_time_ )
						{
							// file was modified, reload
							item->file_time_ = file_time;
							item->data_ = std::make_unique<T>( p );
						}
					}
					catch( std::exception& e ) {
						xo_error( "Error updating resource: "s + e.what() );
					}
				}
			}
			else
			{				
				// file not found in cache, create new cached_item
				xo_error_if( !file_exists( p ), "File does not exist: " + to_str( p ) );
				try
				{
					cached_item new_item;
					new_item.file_time_ = last_write_time( p );
					new_item.last_checked_time_ = std::time( nullptr );
					new_item.data_ = std::make_unique<T>( p );
					auto [it, is_new_item] = cached_items_.insert( std::make_pair( p, std::move( new_item ) ) );
					item = &it->second;
				}
				catch ( std::exception& e ) {
					xo_error( "Error creating resource: "s + e.what() );
				}
			}

			// at this point, item is valid and we can return a copy
			return std::make_unique<T>( *item->data_ );
		}

	private:
		static constexpr double min_check_modified_interval = 3.0; // interval in seconds

		std::mutex mutex_;
		struct cached_item {
			time_t file_time_;
			time_t last_checked_time_;
			u_ptr<T> data_;
		};
		std::map< PathT, cached_item > cached_items_;
	};
}
