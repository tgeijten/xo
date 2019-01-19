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
		using create_func_t = std::function< std::unique_ptr< T >( Args... ) >;

		/// register type U
		template< typename U >
		void register_type( const std::string& name = clean_type_name<U>() ) {
			func_map_[ name ] = []( Args... args ) { return std::unique_ptr< T >( new U( args... ) ); };
		}

		/// unregister type
		void unregister_type( const std::string& type ) {
			auto it = func_map_.find( type );
			xo_error_if( it == func_map_.end(), "Unknown type: " + type );
			func_map_.erase( it );
		}

		/// create instance of type
		std::unique_ptr< T > create( const std::string& type, Args... args ) {
			auto it = func_map_.find( type );
			xo_error_if( it == func_map_.end(), "Unknown type: " + type );
			return it->second( args... );
		}

		/// try create instance of type, return nullptr if not found
		std::unique_ptr< T > try_create( const std::string& type, Args... args ) {
			auto it = func_map_.find( type );
			return it != func_map_.end() ? it->second( args... ) : nullptr;
		}

		bool empty() const { return func_map_.empty(); }
		bool has_type( const std::string& type ) const { return func_map_.find( type ) != func_map_.end(); }

	private:
		xo::flat_map< std::string, create_func_t > func_map_;
	};

	template< typename F, typename C >
	class scoped_factory_registrant {
	public:
		scoped_factory_registrant( F& f, const string& name = clean_type_name<C>() ) : factory_( f ), name_( name )
		{ factory_.register_class<C>( name_ ); }
		~scoped_factory_registrant() { factory_.unregister_class( name_ ); }
	private:
		F& factory_;
		string name_;
	};
}
