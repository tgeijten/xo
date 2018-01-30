#include "prop_node_tools.h"

namespace xo
{
	void merge_prop_nodes( prop_node& pn, const prop_node& other, bool overwrite )
	{
		for ( auto& o : other )
		{
			auto it = pn.find( o.first );
			if ( it == pn.end() )
				pn.push_back( o.first, o.second );
			else if ( overwrite )
				it->second = o.second;
		}
	}

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
}
