#include "prop_node_tools.h"
#include "xo/system/log.h"

namespace xo
{
	void log_unaccessed( const prop_node& pn, log::level level, size_t depth )
	{
		if ( pn.size() > 0 )
		{
			for ( auto& n : pn )
			{
				if ( n.second.count_unaccessed() > 0 )
				{
					string str = string( depth * 2, ' ' ) + n.first;
					if ( !n.second.is_accessed() )
					{
						if ( n.second.has_value() )
							str += " = " + n.second.raw_value();
						str += " *";
					}
					log::message( level, str );
				}
				log_unaccessed( n.second, level, depth + 1 );
			}
		}
	}

	void to_str_unaccessed( const prop_node& pn, string& str, size_t depth )
	{
		if ( pn.size() > 0 )
		{
			for ( auto& n : pn )
			{
				if ( n.second.count_unaccessed() > 0 )
				{
					str += string( depth * 2, ' ' ) + n.first;
					if ( !n.second.is_accessed() )
					{
						if ( n.second.has_value() )
							str += " = " + n.second.raw_value();
						str += " *";
					}
					str += '\n';
				}
				to_str_unaccessed( n.second, str, depth + 1 );
			}
		}
	}

	string to_str_unaccessed( const prop_node& pn )
	{
		string str;
		str.reserve( 1000 );
		to_str_unaccessed( pn, str, 0 );
		return str;
	}

	pair< bool, string > find_query_to_node( const prop_node* from, const prop_node* to, const char delim )
	{
		if ( from == to )
			return { true, "" };

		for ( auto& child : *from )
		{
			auto result = find_query_to_node( &child.second, to, delim );
			if ( result.first )
				return { true, result.second.empty() ? child.first : child.first + delim + result.second };
		}

		return { false, "" };
	}

	prop_node prop_node_from_arg( int argc, const char* argv[] )
	{
		prop_node pn;

		xo_assert( argc >= 1 );
		pn.set_value( argv[ 0 ] );
		prop_node* value_pn = nullptr;
		for ( int i = 1; i < argc; ++i )
		{
			string arg = argv[ i ];
			if ( str_begins_with( arg, '-' ) )
			{
				value_pn = &pn.push_back( mid_str( arg, 1 ) );
			}
			else if ( value_pn )
			{
				value_pn->set_value( arg );
				value_pn = nullptr;
			}
			else pn[ "" ].set_value( arg );
		}

		return pn;
	}
}
