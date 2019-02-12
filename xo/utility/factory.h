#pragma once

#include <functional>
#include <memory>
#include <string>
#include "xo/system/assert.h"
#include "xo/system/system_tools.h"
#include "xo/container/flat_map.h"

#define XO_FACTORY_REGISTRANT( _factory_, _type_ ) \
	auto _type_##_registrant = ::xo::make_type_resistrant< _type_ >( _factory_ )
#define XO_FACTORY_REGISTRANT_NAMED( _factory_, _type_, _name_ ) \
	auto _name_##_registrant = xo::make_type_resistrant< _type_ >( _factory_, #_name_ )

namespace xo
{
	template< typename T, typename... Args >
	class factory
	{
	public:
		using create_func_t = std::function< std::unique_ptr< T >( Args... ) >;
		using container_t = xo::flat_map< std::string, create_func_t >;
		using const_iterator = typename container_t::const_iterator;

		/// register type U
		template< typename U >
		void register_type( const std::string& type_id = clean_type_name<U>() ) {
			xo_error_if( func_map_.has_key( type_id ), "Type already registered: " + type_id );
			func_map_[ type_id ] = []( Args... args ) { return std::unique_ptr< T >( new U( args... ) ); };
		}

		/// unregister type
		void unregister_type( const std::string& type_id ) {
			auto it = find( type_id );
			xo_error_if( it == end(), "Unknown type: " + type_id );
			func_map_.erase( it );
		}

		/// create instance of type
		std::unique_ptr< T > create( const std::string& type_id, Args... args ) {
			auto it = find( type_id );
			xo_error_if( it == end(), "Unknown type: " + type_id );
			return it->second( args... );
		}

		/// try create instance of type, return nullptr if not found
		std::unique_ptr< T > try_create( const std::string& type_id, Args... args ) {
			auto it = find( type_id );
			return it != end() ? it->second( args... ) : nullptr;
		}

		bool empty() const { return func_map_.empty(); }
		bool has_type( const std::string& type_id ) const { return func_map_.find( type_id ) != func_map_.end(); }

		const_iterator begin() const { return func_map_.begin(); }
		const_iterator end() const { return func_map_.end(); }
		const_iterator find( const std::string& type ) { return func_map_.find( type ); }

	private:
		container_t func_map_;
	};

	template< typename T, typename F >
	class scoped_type_registrant {
	public:
		scoped_type_registrant( F& f, const string& name = clean_type_name<T>() ) : factory_( f ), name_( name ) { factory_.register_type<T>( name_ ); }
		~scoped_type_registrant() { factory_.unregister_type( name_ ); }
	private:
		F& factory_;
		string name_;
	};

	template< typename T, typename F > scoped_type_registrant< T, F > make_type_resistrant( F& f ) {
		return typename scoped_type_registrant< T, F >( f, clean_type_name<T>() );
	}

	template< typename T, typename F > scoped_type_registrant< T, F > make_type_resistrant( F& f, const string& type_id ) {
		return typename scoped_type_registrant< T, F >( f, type_id );
	}
}
