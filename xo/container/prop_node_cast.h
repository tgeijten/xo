#pragma once

namespace xo
{
	template< typename T, typename E = void > struct prop_node_cast;
	template< typename T > struct is_prop_node_constructable { static const bool value = false; };
}

#define IS_PROP_NODE_CONSTRUCTABLE( _class_ ) \
template<> struct ::xo::is_prop_node_constructable< _class_ > { static const bool value = true; };
