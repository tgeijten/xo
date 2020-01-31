#include "prop_node_tools.h"
#include "xo/system/log.h"
#include "xo/numerical/math.h"
#include <sstream>

namespace xo
{
	size_t prop_node_align_width( const prop_node& pn, const size_t indent, const size_t depth )
	{
		size_t width = 0;
		for ( auto& child : pn )
		{
			width = max( width, depth * indent + child.first.size() );
			width = max( width, prop_node_align_width( child.second, indent, depth + 1 ) );
		}
		return width;
	}

	void log_prop_node( const prop_node& pn, const size_t indent, const size_t depth, size_t align )
	{
		if ( align == 0 )
			align = prop_node_align_width( pn, 2, depth );

		for ( const auto& [key, value] : pn )
		{
			std::stringstream str;
			log::level l = log::level::info;
			str << string( depth * 2, ' ' );
			if ( str_begins_with( key, '@' ) && key.size() >= 2 )
			{
				str << mid_str( key, 2 );
				l = static_cast<log::level>( int( key[ 1 ] - '0' ) );

			}
			else str << key;

			if ( value.has_value() || value.size() == 0 )
				str << string( align - str.str().size(), ' ' ) << " = " << value.raw_value();
			log::log_string( l, str.str() );
			log_prop_node( value, indent, depth + 1, align );
		}
	}

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
				value_pn = &pn.add_child( mid_str( arg, 1 ) );
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
