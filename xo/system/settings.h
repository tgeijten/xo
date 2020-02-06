#pragma once

#include "xo/string/string_type.h"
#include "xo/container/prop_node.h"
#include "xo/filesystem/path.h"
#include "xo/system/version.h"
#include "xo/system/log.h"

namespace xo
{
	class XO_API settings
	{
	public:
		settings( prop_node schema, const path& filename = path(), const version& current_version = version() );

		/// Get setting of type T
		template< typename T > T get( const string& id ) const;

		/// Set setting of type T
		template< typename T > bool set( const string& id, T value );

		/// Find a setting in the scheme
		const prop_node* try_find_setting( const string& id ) const;

		/// Data access
		const prop_node& data() const { return data_; }
		prop_node& data() { return data_; }

		/// Schema access
		const prop_node& schema() const { return schema_; }

		/// Load settings from a file
		void load( const path& filename = path() );

		/// Save settings to a file
		void save( const path& filename = path() );

		const path& data_file() const { return data_file_; }

		/// Reset to default
		void reset();

		const version& data_version() const { return data_version_; }
		const version& current_version() const { return current_version_; }

	private:
		bool try_set( const string& id, const prop_node& value );
		void set_recursive( const string& id, const prop_node& data );

		prop_node data_;
		const prop_node schema_;
		path data_file_;
		version data_version_;
		version current_version_;
	};

	//
	// IMPLEMENTATION
	//

	template< typename T > T xo::settings::get( const string& id ) const
	{
		if ( auto data_node = data_.try_get_query( id ) )
			return data_node->get<T>();
		else if ( auto* schema_node = try_find_setting( id ) )
			return schema_node->get< T >( "default" );
		else xo_error( "Undefined setting: " + id );
	}

	template< typename T > bool xo::settings::set( const string& id, T value )
	{
		return try_set( id, to_prop_node( value ) );
	}
}
