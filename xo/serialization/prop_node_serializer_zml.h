#pragma once

#include "prop_node_serializer.h"

namespace xo
{
	struct XO_API prop_node_serializer_zml : prop_node_serializer
	{
		prop_node_serializer_zml() : prop_node_serializer() {}
		prop_node_serializer_zml( const prop_node& pn, error_code* ec = nullptr, path file_folder = path() ) : prop_node_serializer( pn, ec, file_folder ) {}
		prop_node_serializer_zml( prop_node& pn, error_code* ec = nullptr, path file_folder = path() ) : prop_node_serializer( pn, ec, file_folder ) {}

		virtual std::istream& read_stream( std::istream& str ) override;
		virtual std::ostream& write_stream( std::ostream& str ) const override;
	};

	struct XO_API prop_node_serializer_zml_concise : prop_node_serializer_zml
	{
		prop_node_serializer_zml_concise() : prop_node_serializer_zml() {}
		prop_node_serializer_zml_concise( const prop_node& pn, error_code* ec = nullptr, path file_folder = path() ) : prop_node_serializer_zml( pn, ec, file_folder ) {}
		prop_node_serializer_zml_concise( prop_node& pn, error_code* ec = nullptr, path file_folder = path() ) : prop_node_serializer_zml( pn, ec, file_folder ) {}

		virtual std::ostream& write_stream( std::ostream& str ) const override;
	};

	XO_API prop_node load_zml( const path& filename, error_code* ec = nullptr, path parent_folder = path() );
	XO_API prop_node parse_zml( const char* str, error_code* ec = nullptr );
}
