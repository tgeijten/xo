#pragma once

#include "xo/xo_types.h"
#include "xo/filesystem/path.h"
#include <iosfwd>
#include <vector>

namespace xo
{
	struct XO_API prop_node_serializer
	{
		prop_node_serializer();
		prop_node_serializer( const prop_node& pn, error_code* ec = nullptr, const path& file_folder = path() );
		prop_node_serializer( prop_node& pn, error_code* ec = nullptr, const path& file_folder = path() );
		virtual ~prop_node_serializer() {}

		virtual std::istream& read_stream( std::istream& str ) = 0;
		virtual std::ostream& write_stream( std::ostream& str ) const = 0;

		prop_node load_file( const path& filename, error_code* ec = nullptr );
		void save_file( const prop_node& pn, const path& filename, error_code* ec = nullptr );

		prop_node* read_pn_;
		const prop_node* write_pn_;
		error_code* ec_;
		path file_folder_;
		std::vector<path> included_files_;
	};

	inline std::istream& operator>>( std::istream& str, prop_node_serializer& pns ) { return pns.read_stream( str ); }
	inline std::ostream& operator<<( std::ostream& str, const prop_node_serializer& pns ) { return pns.write_stream( str ); }
}
