#pragma once
#include "prop_node_serializer.h"

namespace xo
{
	struct XO_API prop_node_serializer_zml : prop_node_serializer
	{
		// inherit constructors from base class
		using prop_node_serializer::prop_node_serializer;

		virtual std::istream& read_stream( std::istream& str ) override;
		virtual std::ostream& write_stream( std::ostream& str ) override;
	};

	struct XO_API prop_node_serializer_zml_concise : prop_node_serializer_zml
	{
		// inherit constructors from base class
		using prop_node_serializer_zml::prop_node_serializer_zml;
		virtual std::ostream& write_stream( std::ostream& str ) override;
	};

	XO_API prop_node load_zml( const path& filename, error_code* ec = nullptr, path parent_folder = path() );
	XO_API prop_node parse_zml( const char* str, error_code* ec = nullptr );
}
