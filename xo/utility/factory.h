#pragma once

#include <functional>
#include <memory>
#include <string>
#include "xo/system/assert.h"
#include "xo/system/system_tools.h"
#include "xo/container/flat_map.h"

namespace xo
{
	template< typename T, typename... Args >
	class factory
	{
	public:
		typedef std::function< std::unique_ptr< T >( Args... ) > create_func_t;

		// register class
		template< typename U > void register_class( const std::string& name = clean_type_name<U>() ) {
			func_map_[ name ] = []( Args... args ) { return std::unique_ptr< T >( new U( args... ) ); };
		}

		// unregister class
		void unregister_class( const std::string& type ) {
			auto it = func_map_.find( type );
			xo_error_if( it == func_map_.end(), "Unregistered type: " + type );
			func_map_.erase( it );
		}

		// access function
		const create_func_t& operator[]( const std::string& type ) const {
			auto it = func_map_.find( type );
			xo_error_if( it == func_map_.end(), "Unregistered type: " + type );
			return it->second;
		}

		// create instance
		std::unique_ptr< T > create( const std::string& type, Args... args ) const { return ( *this )[ type ]( args... ); }
		std::unique_ptr< T > operator()( const std::string& type, Args... args ) const { return create( type, args... ); }

		// try create instance
		std::unique_ptr< T > try_create( const std::string& type, Args... args ) const
		{ auto it = func_map_.find( type ); return it != func_map_.end() ? it->second( args... ) : nullptr; }

		bool empty() const { return func_map_.empty(); }
		bool has_type( const std::string& type ) const { return func_map_.find( type ) != func_map_.end(); }

	private:
		xo::flat_map< std::string, create_func_t > func_map_;
	};
}
