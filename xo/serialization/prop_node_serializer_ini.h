#pragma once

#include "prop_node_serializer.h"

namespace xo
{
	struct prop_node_serializer_ini : prop_node_serializer
	{
		// inherit constructors from base class
		using prop_node_serializer::prop_node_serializer;

		virtual std::istream& read_stream( std::istream& str ) override;
		virtual std::ostream& write_stream( std::ostream& str ) const override;
	};
}
