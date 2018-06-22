#include "prop_node_tools.h"

namespace xo
{
	void log_unaccessed( const prop_node& pn, log::level level, int depth )
	{
		for ( auto& n : pn )
		{
			if ( n.second.count_unaccessed() > 0 )
			{
				string str = string( depth * 2, ' ' ) + n.first;
				if ( !n.second.is_accessed() )
				{
					if ( n.second.has_value() )
						str += " = " + n.second.get_value();
					str += " *";
				}
				log::message( level, str );
			}
			log_unaccessed( n.second, level, depth + 1 );
		}
	}

	prop_node::pair_t* get_child_by_index( prop_node& pn, index_t& idx )
	{
		for ( auto& child : pn )
		{
			if ( idx == 0 )
				return &child;
			else if ( auto* c = get_child_by_index( child.second, --idx ) )
				return c;
		}
		return nullptr;
	}
}
