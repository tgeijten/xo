#include "prop_node_serializer_ini.h"
#include "../system/error_code.h"

namespace xo
{
	std::ostream& prop_node_serializer_ini::write_stream( std::ostream& str, const prop_node& pn, error_code* ec )
	{
		for ( auto& e : pn )
		{
			if ( e.second.size() > 0 ) // group item
			{
				str << '[' << e.first << ']' << std::endl;
				for ( auto& e2 : e.second )
					str << e2.first << "=" << e2.second.get_value() << std::endl;
			}
			else if ( e.second.has_value() ) // main item
				str << e.first << "=" << e.second.get_value() << std::endl;
		}
		return str;
	}

	xo::prop_node prop_node_serializer_ini::read_stream( std::istream& str, error_code* ec, path parent_folder )
	{
		prop_node pn;
		prop_node* cur_group = &pn;

		while ( str.good() )
		{
			char buf[ 1024 ];
			str.getline( buf, sizeof( buf ) );
			if ( str.fail() && !str.eof() )
				return set_error_or_throw( ec, "Error reading ini file, line too long" ), prop_node();

			string line( buf );

			if ( line.length() == 0 ) // empty line
				continue;

			if ( line[ 0 ] == ';' ) // comment
				continue;

			if ( line.size() > 2 && line[ 0 ] == '[' && line[ line.size() - 1 ] == ']' )
			{
				cur_group = &pn.push_back( line.substr( 1, line.size() - 2 ) );
				continue;
			}

			// must be a key = value line
			auto kvp = make_key_value_str( line );
			xo_error_if( kvp.first == line, "Error loading ini file, expected '='" );
			cur_group->set( kvp.first, kvp.second );
		}
		return pn;
	}
}
