#pragma once
#include "prop_node_serializer.h"

namespace xo
{
	struct XO_API prop_node_serializer_zml : prop_node_serializer
	{
		prop_node_serializer_zml() {}
		virtual ~prop_node_serializer_zml() {}

		virtual prop_node read_stream( std::istream& str, error_code* ec = nullptr, path parent_folder = path() );
		virtual std::ostream& write_stream( std::ostream& str, const prop_node& pn, error_code* ec = nullptr );
	};

	XO_API prop_node load_zml( const path& filename, error_code* ec = nullptr, path parent_folder = path() );
	XO_API prop_node parse_zml( const char* str, error_code* ec = nullptr );
}
