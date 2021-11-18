#include "settings.h"
#include "xo/serialization/serialize.h"
#include "log.h"
#include "xo/numerical/bounds.h"
#include "xo/filesystem/filesystem.h"
#include "assert.h"
#include "xo/utility/optional.h"
#include "xo/container/container_tools.h"
#include "xo/utility/color.h"

namespace xo
{
	settings::settings( prop_node schema, const path& filename, const version& current_version ) :
		schema_( std::move( schema ) ),
		current_version_( current_version )
	{
		load( filename );
	}

	const xo::prop_node* settings::try_find_setting( const string& id ) const
	{
		const auto* schema_node = schema_.try_get_query( id );
		if ( schema_node && schema_node->has_key( "default" ) )
			return schema_node;
		else return nullptr;
	}

	bool settings::try_set( const string& id, const prop_node& value )
	{
		if ( auto* schema_node = try_find_setting( id ) )
		{
			const auto type_pn = schema_node->try_get_child( "type" );
			const string type = type_pn ? type_pn->get_str() : "";

			// check if value is empty or the same as the default
			if ( value == schema_node->get_child( "default" ) || value.empty() )
			{
				data_.erase_query( id );
				return true; // success, don't store value
			}

			// check if the value is valid for the type
			if (
				( type == "float" && !value.try_get<double>() ) ||
				( type == "int" && !value.try_get<int>() ) ||
				( type == "bool" && !value.try_get<bool>() ) ||
				( type == "color" && !value.try_get<xo::color>() ) ||
				( type == "string" && !value.try_get<string>() )
				)
			{
				log::error( "Error setting ", id, ": could not convert '", value.get_str(), "' to ", type );
				return false;
			}

			// check if the setting is part of a specified 'allowed' list
			if ( auto allowed = schema_node->try_get_child( "allowed" ) )
			{
				auto it = xo::find_if( *allowed, [&]( auto& kvp ) { return kvp.second == value; } );
				if ( it == allowed->end() )
				{
					log::warning( "Invalid value for ", id, ": ", value );
					return false; // value not allowed, don't store value
				}
			}

			// check if the setting is within a specified 'range'
			if ( auto range = schema_node->try_get< bounds< double > >( "range" ) )
			{
				auto dvalue = schema_node->get< double >( "default" );
				from_str( value.get_str(), dvalue );
				if ( !range->is_within( dvalue ) )
				{
					log::warning( "Invalid value for ", id, ": ", dvalue, ", restricting to range ", *range );
					range->clamp( dvalue );
				}
				data_.set_query( id, dvalue );
				return true;
			}

			// value can be set
			data_.set_query( id, value );
			return true;
		}
		else
		{
			log::warning( "Undefined setting: " + id );
			return false;
		}
	}

	void settings::set_recursive( const string& id, const prop_node& data )
	{
		if ( auto* pn = try_find_setting( id ) ) // actual setting?
			try_set( id, data );
		else if ( data.size() > 0 ) // settings group?
			for ( auto& child : data )
				set_recursive( id + '.' + child.first, child.second );
	}

	void settings::load( const path& filename )
	{
		if ( !filename.empty() )
			data_file_ = filename;

		if ( !data_file_.empty() )
		{
			if ( file_exists( data_file_ ) )
			{
				prop_node pn = load_file( data_file_ );
				for ( auto& c : pn )
				{
					if ( c.first == "version" )
						data_version_ = c.second.get< version >();
					else set_recursive( c.first, c.second );
				}
			}
			else
			{
				log::info( "Could not find settings file, creating defaults" );
				reset();
				save();
			}
		}
		else xo_error( "Error loading settings: no filename was given" );
	}

	void settings::save( const path& filename )
	{
		if ( !filename.empty() )
			data_file_ = filename;

		if ( !current_version().empty() )
			data_.set( "version", current_version() );

		xo::create_directories( data_file_.parent_path() );
		save_file( data_, data_file_ );
		log::debug( "Saved settings to ", data_file_ );
	}

	void settings::reset()
	{
		data_.clear();
	}
}
