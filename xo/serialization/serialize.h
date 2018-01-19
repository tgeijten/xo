#pragma once
#include "xo/container/prop_node.h"

namespace xo
{
	enum class file_format { unknown, xml, ini, pn };

	template< file_format F >
	struct prop_node_serializer
	{
		prop_node_serializer( prop_node& pn ) : pn_( pn ) {}
		prop_node& pn_;
	};

	XO_API std::ostream& operator<<( std::ostream& str, xo::prop_node_serializer< xo::file_format::xml > p );
	XO_API std::istream& operator>>( std::istream& str, xo::prop_node_serializer< xo::file_format::xml > p );
}
