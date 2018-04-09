#pragma once

#include "prop_node_serializer.h"

namespace xo
{
	struct prop_node_serializer_ini : prop_node_serializer
	{
		prop_node_serializer_ini() {}
		virtual ~prop_node_serializer_ini() {}

		virtual prop_node read_stream( std::istream& str, error_code* ec = nullptr, path parent_folder = path() );
		virtual std::ostream& write_stream( std::ostream& str, const prop_node& pn, error_code* ec = nullptr );
	};
}
