#pragma once

#include "xo/container/prop_node.h"
#include "prop_node_serializer.h"
#include "xo/utility/factory.h"

namespace xo
{
	XO_API factory< prop_node_serializer >& get_serializer_factory();
	XO_API u_ptr< prop_node_serializer > make_serializer( const string& file_type, error_code* ec = nullptr );
	template< class S > void register_serializer( const string& extension ) { get_serializer_factory().register_class< S >( extension ); }

	XO_API prop_node load_file( const path& filename, error_code* ec = nullptr );
	XO_API prop_node load_file( const path& filename, const string& file_type, error_code* ec = nullptr );

	XO_API prop_node load_file_with_include( const path& filename, const string& include_directive = "INCLUDE" );

	XO_API void save_file( const prop_node& pn, const path& filename, error_code* ec = nullptr );
	XO_API void save_file( const prop_node& pn, const path& filename, const string& file_type, error_code* ec = nullptr );
}
