#include "prop_node_tools.h"
#include <fstream>
#include "xo/filesystem/filesystem.h"
#include "xo/string/string_tools.h"

// Include rapidxml.hpp first for xml_node
#include <contrib/rapidxml-1.13/rapidxml.hpp>

// Forward declarations for rapidxml
namespace rapidxml {
	namespace internal {
		template<class OutIt, class Ch> inline OutIt print_cdata_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
		template<class OutIt, class Ch> inline OutIt print_children(OutIt out, const xml_node<Ch> *node, int flags, int indent);
		template<class OutIt, class Ch> inline OutIt print_comment_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
		template<class OutIt, class Ch> inline OutIt print_data_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
		template<class OutIt, class Ch> inline OutIt print_declaration_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
		template<class OutIt, class Ch> inline OutIt print_doctype_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
		template<class OutIt, class Ch> inline OutIt print_element_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
		template<class OutIt, class Ch> inline OutIt print_pi_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
	}
}
#include <contrib/rapidxml-1.13/rapidxml_print.hpp>

namespace xo
{
	XO_API prop_node load_file( const path& filename )
	{
		if ( filename.extension() == "xml" )
			return load_xml( filename );
		else return load_prop( filename );
	}

	prop_node get_rapid_xml_node( rapidxml::xml_node<>* node )
	{
		// make new prop_node
		prop_node pn = make_prop_node( node->value() );

		// add attributes
		for ( rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute() )
			pn.push_back( attr->name(), attr->value() );

		// add child nodes
		for ( rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling() )
		{
			if ( child->name_size() > 0 )
				pn.push_back( child->name(), get_rapid_xml_node( child ) );
		}

		return pn;
	}

	void set_rapid_xml_node( rapidxml::xml_document<>& doc, rapidxml::xml_node<>* xmlnode, const prop_node& pn )
	{
		if ( pn.has_value() )
			xmlnode->value( pn.get_value().c_str() );

		for ( auto& child : pn )
		{
			xmlnode->append_node( doc.allocate_node( rapidxml::node_element, child.first.c_str() ) );
			set_rapid_xml_node( doc, xmlnode->last_node(), child.second );
		}
	}

	XO_API prop_node load_xml( const path& filename, error_code* ec )
	{
		string file_contents = load_string( filename, ec );
		if ( ec && ec->error() )
			return prop_node();

		try
		{
			rapidxml::xml_document<> doc;
			doc.parse< 0 >( &file_contents[ 0 ] ); // not officially supported but probably safe

			if ( doc.first_node() )
			{
				prop_node pn;
				pn.push_back( doc.first_node()->name(), get_rapid_xml_node( doc.first_node() ) );
				return pn;
			}
			else return prop_node();
		}
		catch ( std::exception& e )
		{
			if ( try_set_error( ec, e.what() ) )
				return prop_node();
			else throw e;
		}
	}

	XO_API void save_xml( const prop_node& pn, const path& filename )
	{
		rapidxml::xml_document<> doc;
		set_rapid_xml_node( doc, &doc, pn );
		std::ofstream ostr( filename.str() );
		ostr << doc;
	}

	bool is_valid_prop_label( const string& s )
	{
		return ( s.size() > 0 && isalpha( s[ 0 ] ) );
	}

	string get_prop_token( char_stream& str )
	{
		while ( true )
		{
			string t = str.get_token( "={}%", "\"'" );
			if ( t == "%" )
			{
				// comment: skip rest of line
				str.get_line();
				continue;
			}
			else return trim_str( t );
		}
	}

	void read_prop_node( char_stream& str, prop_node& parent )
	{
		string t = get_prop_token( str );
		if ( t == "=" )
		{
			parent.set_value( get_prop_token( str ) );
		}
		else if ( t == "{" )
		{
			while ( str.good() && t != "}" )
			{
				t = get_prop_token( str );
				if ( is_valid_prop_label( t ) )
					read_prop_node( str, parent.push_back( t ) );
				else if ( t != "}" )
					xo_error( "Invalid token: " + t );
			}
		}
	}

	prop_node parse_prop( char_stream& str, error_code* ec )
	{
		prop_node root;
		string t = get_prop_token( str );
		while ( is_valid_prop_label( t ) )
		{
			read_prop_node( str, root.push_back( t ) );
			t = get_prop_token( str );
		}
		return root;
	}

	XO_API prop_node parse_prop( const char* str, error_code* ec )
	{
		return parse_prop( char_stream( str, "\n\r\t\v;" ), ec );
	}

	XO_API prop_node load_prop( const path& filename, error_code* ec )
	{
		auto str = load_string( filename, ec );
		if ( ec && ec->error() )
			return prop_node();
		else return parse_prop( str.c_str(), ec );
	}

	XO_API prop_node load_ini( const path& filename )
	{
		prop_node pn;
		prop_node* cur_group = &pn;

		auto str = load_char_stream( filename );
		while ( str.good() )
		{
			string line = trim_str( str.get_line() );

			if ( line.length() == 0 ) // empty line
				continue;

			if ( line[0] == '#' ) // comment
			{
				str.get_line();
				continue;
			}

			if ( line.size() > 2 && line[ 0 ] == '[' && line[ line.size() -1 ] == ']' )
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

	XO_API void save_ini( const prop_node& pn, const path& filename )
	{
		std::ofstream str( filename.str() );
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
	}

	void write_prop_none( std::ostream& str, const string& label, const prop_node& pn, int level, bool readable )
	{
		string indent = readable ? string( level, '\t' ) : "";
		string newline = readable ? "\n" : " ";
		string assign = readable ? " = " : "=";

		str << indent << try_quoted( label );
		if ( pn.has_value() || pn.empty() )
			str << assign << ( pn.empty() ? "\"\"" : try_quoted( pn.get_value() ) );
		str << newline;
		if ( pn.size() > 0 )
		{
			str << indent << "{" << newline; // #TODO only do newline when needed
			for ( auto& node : pn )
				write_prop_none( str, node.first, node.second, level + 1, readable );
			str << indent << "}" << newline;
		}
	}

	XO_API bool save_prop( const prop_node& pn, const path& filename, bool readable )
	{
		std::ofstream str( filename.str(), std::ios_base::out | std::ios_base::trunc );
		if ( str.good() )
		{
			for ( auto& node : pn )
				write_prop_none( str, node.first, node.second, 0, readable );
			return true;
		}
		else return false;
	}

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

	void resolve_include_files( prop_node &pn, const path &filename, const string& include_directive, int level )
	{
		for ( auto iter = pn.begin(); iter != pn.end(); )
		{
			if ( iter->first == include_directive )
			{
				// load included file using filename path
				path include_path = filename.parent_path() / iter->second.get< path >( "file" );
				bool merge_children = iter->second.get< bool >( "merge_children", false );
				auto included_props = load_file_with_include( include_path, include_directive, level + 1 );
				xo_error_if( included_props.size() != 1, "Included files must have a single root node" );
				auto& included_children = included_props.begin()->second;

				// remove the include node
				iter = pn.erase( iter );

				// merge or include, depending on options
				if ( merge_children )
				{
					merge_prop_nodes( pn, included_children, false );
					iter = pn.begin(); // reset the iterator, which has become invalid after merge
				}
				else
				{
					// insert the children at the INCLUDE spot
					iter = pn.insert( iter, included_props.begin(), included_props.end() );
				}
			}
			else
			{
				// search in children
				resolve_include_files( iter->second, filename, include_directive, level );
				++iter;
			}
		}
	}

	prop_node load_file_with_include( const path& filename, const string& include_directive, int level )
	{
		xo_error_if( level >= 100, "Exceeded maximum include level, check for loops in includes" );

		prop_node pn = load_file( filename );
		resolve_include_files( pn, filename, include_directive, level );
		pn.clear_accessed_recursively();

		return pn;
	}

	XO_API void log_unaccessed( const prop_node& pn, log::level level, int depth )
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
