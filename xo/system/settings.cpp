#include "settings.h"
#include "xo/serialization/serialize.h"
#include "log.h"
#include "xo/numerical/bounds.h"
#include "xo/filesystem/filesystem.h"
#include "assert.h"

namespace xo
{
	settings::settings( prop_node schema, const path& filename ) :
	schema_( std::move( schema ) )
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
		log::info( "Setting ", id, " to ", value.get_value() );

		if ( auto* schema_node = try_find_setting( id ) )
		{
			// check if value is different from default
			if ( value == schema_node->get_child( "default" ) )
				return true; // success, don't store value

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
				auto dvalue = from_str< double >( value.get_value(), schema_node->get< double >( "default" ) );
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

	bool settings::set_recursive( const prop_node& data, string prefix )
	{
		if ( auto* pn = try_find_setting( prefix ) )
		{
			return try_set( prefix, data );
		}
		else
		{
			bool ok = true;
			for ( auto& child : data )
				ok &= set_recursive( child.second, prefix.empty() ? child.first : prefix + '.' + child.first );
			return ok;
		}
	}

	void settings::load( const path& filename )
	{
		if ( !filename.empty() )
			data_file_ = filename;

		if ( !data_file_.empty() )
			set( load_file( data_file_ ) );
	}

	void settings::save( const path& filename )
	{
		if ( !filename.empty() )
			data_file_ = filename;
		xo::create_directories( data_file_.parent_path() );
		save_file( data_, data_file_ );
		log::info( "Saved settings to ", data_file_ );
	}
}
