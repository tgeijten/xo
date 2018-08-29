#include "settings.h"
#include "xo/serialization/serialize.h"
#include "log.h"
#include "xo/numerical/bounds.h"
#include "xo/filesystem/filesystem.h"
#include "assert.h"

namespace xo
{
	settings::settings( prop_node schema, const path& filename ) :
	schema_( std::move( schema ) ),
	data_file_( filename )
	{
		if ( file_exists( data_file_ ) )
			data_ = load_file( data_file_ );
		fix_settings( data_, schema_ );
	}

	void settings::set( prop_node data )
	{
		data_ = std::move( data );
		fix_settings( data_, schema_ );
	}

	void settings::fix_settings( prop_node& data_pn, const prop_node& schema_pn )
	{
		for ( auto& item : schema_pn )
		{
			bool is_group = !item.second.has_key( "default" );
			if ( auto data_child = data_pn.try_get_child( item.first ) )
			{
				if ( is_group )
					fix_settings( *data_child, item.second );
				else fix_setting( *data_child, item.second );
			}
			else
			{
				if ( is_group )
					fix_settings( data_pn.push_back( item.first ), item.second );
				else data_pn.push_back( item.first, item.second.get_child( "default" ) );
			}
		}
	}

	void settings::fix_setting( prop_node& setting, const prop_node& schema )
	{
		if ( auto allowed = schema.try_get_child( "allowed" ) )
		{
			bool is_allowed = false;
			for ( auto& v : *allowed )
				is_allowed |= v.second.get_value() == setting.get_value();
			if ( !is_allowed )
			{
				log::warning( "Invalid setting: ", setting.get_value(), ", restoring to default " );
				setting.set_value( schema[ "default" ].get_value() );
			}
		}
		if ( auto range = schema.try_get< boundsd >( "range" ) )
		{
			auto value = setting.get<double>();
			if ( !range->is_within( value ) )
			{

				log::warning( "Invalid value: ", value, ", restricting to range ", *range );
				setting.set( range->clamped( value ) );
			}
		}
	}

	void settings::load( const path& filename )
	{
		if ( !filename.empty() )
			data_file_ = filename;
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
