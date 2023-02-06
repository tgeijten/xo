#include "prop_node_serializer_xml.h"

// Include rapidxml.hpp first for xml_node
#include <contrib/rapidxml-1.13/rapidxml.hpp>

// Forward declarations for rapidxml
namespace rapidxml {
	namespace internal {
		template<class OutIt, class Ch> inline OutIt print_cdata_node( OutIt out, const xml_node<Ch>* node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_children( OutIt out, const xml_node<Ch>* node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_comment_node( OutIt out, const xml_node<Ch>* node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_data_node( OutIt out, const xml_node<Ch>* node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_declaration_node( OutIt out, const xml_node<Ch>* node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_doctype_node( OutIt out, const xml_node<Ch>* node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_element_node( OutIt out, const xml_node<Ch>* node, int flags, int indent );
		template<class OutIt, class Ch> inline OutIt print_pi_node( OutIt out, const xml_node<Ch>* node, int flags, int indent );
	}
}

#include <contrib/rapidxml-1.13/rapidxml_print.hpp>

#include "xo/container/prop_node.h"
#include <iostream>

namespace xo
{
	prop_node get_rapid_xml_node( rapidxml::xml_node<>* node )
	{
		// make new prop_node
		prop_node pn = to_prop_node( node->value() );

		// add attributes
		for ( rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute() )
			pn.add_key_value( attr->name(), attr->value() );

		// add child nodes
		for ( rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling() )
		{
			if ( child->name_size() > 0 )
				pn.add_child( child->name(), get_rapid_xml_node( child ) );
		}

		return pn;
	}

	void set_rapid_xml_node( rapidxml::xml_document<>& doc, rapidxml::xml_node<>* xmlnode, const prop_node& pn )
	{
		if ( pn.has_value() )
			xmlnode->value( pn.get_str().c_str() );

		for ( auto& child : pn )
		{
			xmlnode->append_node( doc.allocate_node( rapidxml::node_element, !child.first.empty() ? child.first.c_str() : "_" ) );
			set_rapid_xml_node( doc, xmlnode->last_node(), child.second );
		}
	}

	std::istream& prop_node_serializer_xml::read_stream( std::istream& str )
	{
		xo_assert( read_pn_ );
		rapidxml::xml_document<> doc;
		std::string file_contents( std::istreambuf_iterator<char>( str ), {} );
		doc.parse< 0 >( &file_contents[0] ); // not officially supported but probably safe
		if ( doc.first_node() )
			read_pn_->add_child( doc.first_node()->name(), get_rapid_xml_node( doc.first_node() ) );
		return str;
	}

	std::ostream& prop_node_serializer_xml::write_stream( std::ostream& str ) const
	{
		rapidxml::xml_document<> doc;
		set_rapid_xml_node( doc, &doc, *write_pn_ );
		return str << doc;
	}
}
