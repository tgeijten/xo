#include "settings.h"
#include "xo/serialization/serialize.h"
#include "log.h"

namespace xo
{
	void extract_settings( const prop_node& src, prop_node& trg )
	{
		for ( auto& kpn : src )
		{
			if ( kpn.second.has_key( "_label_" ) )
				trg.push_back( kpn.first, kpn.second.get_value() );
			else extract_settings( kpn.second, trg.push_back( kpn.first ) );
		}
	}

	void inject_settings( const prop_node& src, prop_node& trg )
	{
		for ( auto& kpn : src )
		{
			if ( auto* pn = trg.try_get_child( kpn.first ) )
			{
				if ( pn->has_key( "_label_" ) )
					pn->set_value( kpn.second.get_value() );
				else inject_settings( kpn.second, *pn );
			}
			else log::warning( "Ignored setting: ", kpn.first );
		}
	}

	void settings::load( const path& filename )
	{
		auto pn = load_file( filename );
		inject_settings( data_, pn );
	}

	void settings::save( const path& filename ) const
	{
		prop_node pn;
		extract_settings( data_, pn );
		save_file( pn, filename, detect_file_format( filename ) );
	}
}
