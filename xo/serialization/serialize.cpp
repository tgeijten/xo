#include "serialize.h"

// Include rapidxml.hpp first for xml_node
#include <contrib/rapidxml-1.13/rapidxml.hpp>

// Forward declarations for rapidxml
namespace rapidxml {
	namespace internal {
		template<class OutIt, class Ch> inline OutIt print_cdata_node( OutIt out, const xml_node<Ch> *node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_children( OutIt out, const xml_node<Ch> *node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_comment_node( OutIt out, const xml_node<Ch> *node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_data_node( OutIt out, const xml_node<Ch> *node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_declaration_node( OutIt out, const xml_node<Ch> *node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_doctype_node( OutIt out, const xml_node<Ch> *node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_element_node( OutIt out, const xml_node<Ch> *node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_pi_node( OutIt out, const xml_node<Ch> *node, int flags, int indent );
	}
}
#include <contrib/rapidxml-1.13/rapidxml_print.hpp>

namespace xo
{
	prop_node get_rapid_xml_node2( rapidxml::xml_node<>* node )
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
				pn.push_back( child->name(), get_rapid_xml_node2( child ) );
		}

		return pn;
	}

	void set_rapid_xml_node2( rapidxml::xml_document<>& doc, rapidxml::xml_node<>* xmlnode, const prop_node& pn )
	{
		if ( pn.has_value() )
			xmlnode->value( pn.get_value().c_str() );

		for ( auto& child : pn )
		{
			xmlnode->append_node( doc.allocate_node( rapidxml::node_element, child.first.c_str() ) );
			set_rapid_xml_node2( doc, xmlnode->last_node(), child.second );
		}
	}

	XO_API std::istream& operator>>( std::istream& str, prop_node_serializer< file_format::xml > p )
	{
		std::string file_contents( std::istreambuf_iterator<char>( str ), {} );

		rapidxml::xml_document<> doc;
		doc.parse< 0 >( &file_contents[ 0 ] ); // not officially supported but probably safe
		if ( doc.first_node() )
			p.pn_.push_back( doc.first_node()->name(), get_rapid_xml_node2( doc.first_node() ) );
		return str;
	}

	XO_API std::ostream& operator<<( std::ostream& str, const prop_node_serializer< file_format::xml > p )
	{
		rapidxml::xml_document<> doc;
		set_rapid_xml_node2( doc, &doc, p.pn_ );
		return str << doc;
	}
}

