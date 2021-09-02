#pragma once

#include "xo/filesystem/filesystem.h"
#include "xo/utility/pointer_types.h"
#include "xo/container/container_tools.h"
#include "xo/string/string_type.h"

#include <mutex>
#include <shared_mutex>
#include <ctime>
#include <map>

namespace xo
{
	template< typename T, typename PathT >
	struct file_resource_cache
	{
		using create_function = std::function<T* ( const PathT& )>;
		using clone_function = std::function<T* ( const T& )>;

		file_resource_cache( create_function cr = default_create, clone_function cl = default_clone ) :
			create_( std::move( cr ) ), clone_( std::move( cl ) ) {}

		std::unique_ptr<T> operator()( const PathT& p )
		{
			std::shared_lock read_lock( mutex_ );

			cached_item* item = find( p );
			if ( item )
			{
				// check how long ago we checked if the file was modified
				auto wall_time = std::time( nullptr );
				if ( std::difftime( wall_time, item->last_checked_time_ ) > min_check_modified_interval )
				{
					// we haven't checked for a while
					read_lock.unlock();
					try
					{
						// check if the file has been modified
						std::unique_lock write_lock( mutex_ );
						item->last_checked_time_ = wall_time;
						auto file_time = last_write_time( p );
						if ( file_time != item->file_time_ )
						{
							// file was modified, reload
							item->file_time_ = file_time;
							item->data_.reset( create_( p ) );
						}
					}
					catch( std::exception& e ) {
						xo_error( "Error reading " + to_str( p ) + ": " + e.what() );
					}
					read_lock.lock();
				}
			}
			else
			{				
				// file not found in cache, create new cached_item
				xo_error_if( !file_exists( p ), "File does not exist: " + to_str( p ) );

				read_lock.unlock();
				try
				{
					std::unique_lock write_lock( mutex_ );
					// find the item again, to make sure another thread didn't just create it
					item = find( p );
					if ( !item ) 
					{
						cached_item new_item;
						new_item.file_time_ = last_write_time( p );
						new_item.last_checked_time_ = std::time( nullptr );
						new_item.data_.reset( create_( p ) );
						auto [it, is_new_item] = cached_items_.insert( std::make_pair( p, std::move( new_item ) ) );
						item = &it->second;
					}
				}
				catch ( std::exception& e ) {
					xo_error( "Error reading " + to_str( p ) + ": " + e.what() );
				}
				read_lock.lock();
			}

			// at this point, read_load holds the mutex and item is valid
			return std::unique_ptr<T>( clone_( *item->data_ ) );
		}

	private:
		static T* default_create( const PathT& f ) { return new T( f ); }
		static T* default_clone( const T& o ) { return new T( o ); }

		static constexpr double min_check_modified_interval = 3.0; // interval in seconds

		struct cached_item {
			time_t file_time_;
			time_t last_checked_time_;
			u_ptr<T> data_;
		};

		std::shared_mutex mutex_;
		std::map< PathT, cached_item > cached_items_;

		cached_item* find( const PathT& p ) {
			auto it = cached_items_.find( p );
			if ( it != cached_items_.end() )
				return &it->second;
			else return nullptr;
		}

		create_function create_;
		clone_function clone_;
	};
}
