#pragma once

#include "xo/container/prop_node.h"
#include "xo/system/error_code.h"
#include "xo/filesystem/path.h"

namespace xo
{
	struct XO_API prop_node_serializer
	{
		prop_node_serializer( error_code* ec = nullptr, path file_folder = path() );
		prop_node_serializer( const prop_node& pn, error_code* ec = nullptr, path file_folder = path() );
		prop_node_serializer( prop_node& pn, error_code* ec = nullptr, path file_folder = path() );
		virtual ~prop_node_serializer() {}

		virtual std::istream& read_stream( std::istream& str ) = 0;
		virtual std::ostream& write_stream( std::ostream& str ) = 0;

		prop_node load_file( const path& filename, error_code* ec = nullptr );
		void save_file( const prop_node& pn, const path& filename, error_code* ec = nullptr );

		prop_node* read_pn_;
		const prop_node* write_pn_;
		error_code* ec_;
		path file_folder_;
	};

	inline std::istream& operator>>( std::istream& str, prop_node_serializer& pns ) { return pns.read_stream( str ); }
	inline std::ostream& operator<<( std::ostream& str, prop_node_serializer& pns ) { return pns.write_stream( str ); }
}
