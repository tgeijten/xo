#pragma once

#include "prop_node_serializer.h"

namespace xo
{
	struct XO_API prop_node_serializer_xml : public prop_node_serializer
	{
		prop_node_serializer_xml() : prop_node_serializer() {}
		prop_node_serializer_xml( const prop_node& pn, error_code* ec = nullptr, path file_folder = path() ) : prop_node_serializer( pn, ec, file_folder ) {}
		prop_node_serializer_xml( prop_node& pn, error_code* ec = nullptr, path file_folder = path() ) : prop_node_serializer( pn, ec, file_folder ) {}

		virtual std::istream& read_stream( std::istream& str ) override;
		virtual std::ostream& write_stream( std::ostream& str ) const override;
	};
}
