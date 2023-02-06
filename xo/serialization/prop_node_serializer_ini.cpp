#include "prop_node_serializer_ini.h"

#include "xo/system/error_code.h"
#include "xo/container/prop_node.h"
#include "xo/string/string_tools.h"
#include <iostream>

namespace xo
{
	std::ostream& prop_node_serializer_ini::write_stream( std::ostream& str ) const
	{
		xo_assert( write_pn_ );
		for ( auto& e : *write_pn_ )
		{
			if ( e.second.size() > 0 ) // group item
			{
				str << '[' << e.first << ']' << std::endl;
				for ( auto& e2 : e.second )
					str << e2.first << "=" << e2.second.get_str() << std::endl;
			}
			else if ( e.second.has_value() ) // main item
				str << e.first << "=" << e.second.get_str() << std::endl;
		}
		return str;
	}

	std::istream& prop_node_serializer_ini::read_stream( std::istream& str )
	{
		xo_assert( read_pn_ );
		prop_node* cur_group = read_pn_;

		while ( str.good() )
		{
			char buf[1024];
			str.getline( buf, sizeof( buf ) );
			if ( str.fail() && !str.eof() )
				return set_error_or_throw( ec_, "Error reading ini file, line too long" ), str;

			string line( buf );

			if ( line.length() == 0 ) // empty line
				continue;

			if ( line[0] == ';' ) // comment
				continue;

			if ( line.size() > 2 && line[0] == '[' && line[line.size() - 1] == ']' )
			{
				cur_group = &read_pn_->add_child( line.substr( 1, line.size() - 2 ) );
				continue;
			}

			// must be a key = value line
			auto kvp = make_key_value_str( line );
			xo_error_if( kvp.first == line, "Error loading ini file, expected '='" );
			cur_group->set( kvp.first, kvp.second );
		}
		return str;
	}
}
