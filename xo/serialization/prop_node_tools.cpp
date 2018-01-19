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
#include "serialize.h"

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
