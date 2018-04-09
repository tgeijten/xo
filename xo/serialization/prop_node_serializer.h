#pragma once

#include "xo/container/prop_node.h"
#include "xo/system/error_code.h"
#include "xo/filesystem/path.h"

namespace xo
{
	struct XO_API prop_node_serializer
	{
		prop_node_serializer() {}
		virtual ~prop_node_serializer() {}

		virtual prop_node read_stream( std::istream& str, error_code* ec = nullptr, path parent_folder = path() ) = 0;
		virtual std::ostream& write_stream( std::ostream& str, const prop_node& pn, error_code* ec = nullptr ) = 0;
	};
}
