#include "settings.h"
#include "xo/serialization/serialize.h"
#include "log.h"

namespace xo
{
	void extract_settings( const prop_node& src, prop_node& trg )
	{
		if ( !src.has_value() )
		{
			for ( auto& kpn : src )
				extract_settings( kpn.second, trg.push_back( kpn.first ) );
		}
		else trg.set_value( src.get_value() );
	}

	void inject_settings( const prop_node& src, prop_node& trg )
	{
		if ( !src.has_value() )
		{
			for ( auto& kpn : src )
			{
				if ( auto* pn = trg.try_get_child( kpn.first ) )
					inject_settings( kpn.second, *pn );
				else log::warning( "Ignored setting: ", kpn.first );
			}

		}
		else trg.set_value( src.get_value() );
	}

	void settings::inject_settings( const prop_node& settings )
	{
		::xo::inject_settings( settings, data_ );
	}

	xo::prop_node settings::extract_settings() const
	{
		prop_node pn;
		::xo::extract_settings( data_, pn );
		return pn;
	}

	void settings::load( const path& filename )
	{
		auto pn = load_file( filename );
		inject_settings( pn );
	}

	void settings::save( const path& filename ) const
	{
		save_file( extract_settings(), filename );
	}

	void settings::set_meta_data( prop_node& pn, const string& label, const string& info, type_class t )
	{
		pn[ "_label_" ] = label;
		pn[ "_info_" ] = info;
		pn[ "_type_" ] = t;
	}
}
