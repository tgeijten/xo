#include "prop_node.h"
#include "xo/serialization/prop_node_tools.h"

#include <stdio.h>
#include <memory>
#include <iomanip>
#include <algorithm>

namespace xo
{
	prop_node::prop_node( const char* pn ) : accessed_flag( false )
	{
		*this = parse_prop( pn );
	}

	prop_node g_empty_prop_node;
	XO_API const prop_node& empty_prop_node()
	{
		return g_empty_prop_node;
	}

	int get_align_width( const prop_node& pn, int depth )
	{
		int width = 0;
		for ( auto& child : pn )
		{
			width = std::max( width, depth * 2 + int( child.first.size() ) );
			width = std::max( width, get_align_width( child.second, depth + 1 ) );
		}
		return width;
	}

	std::ostream& to_stream( std::ostream& str, const prop_node& pn, int depth, int align )
	{
		if ( align == 0 )
			align = get_align_width( pn, depth );

		for ( auto& child : pn )
		{
			string key_str( depth * 2, ' ' );
			key_str += child.first;

			str << std::left << std::setw( align ) << key_str;
			if ( child.second.has_value() || child.second.size() == 0 )
				str << std::setw( 0 ) << " = " << child.second.get_value();
			str << std::endl;

			to_stream( str, child.second, depth + 1, align );
		}

		return str;
	}

	bool prop_node::operator==( const prop_node& other )
	{
		if ( value != other.value )
			return false;

		if ( size() != other.size() )
			return false;

		for ( size_t i = 0; i < children.size(); ++i )
		{
			if ( children[i].first != other.children[i].first )
				return false;
			if ( children[i].second != other.children[i].second )
				return false;
		}

		return true;
	}
}
