#include "prop_node_serializer.h"

#include "xo/system/error_code.h"

#include <fstream>

namespace xo
{
	prop_node prop_node_serializer::load_file( const path& filename, error_code* ec )
	{
		std::ifstream str( filename.string() );
		if ( str )
			return read_stream( str, ec, filename.parent_path() );
		else return set_error_or_throw( ec, "Cannot open " + filename.string() ), prop_node();
	}

	void prop_node_serializer::save_file( const prop_node& pn, const path& filename, error_code* ec )
	{
		std::ofstream str( filename.string() );
		if ( str )
			write_stream( str, pn, ec );
		else set_error_or_throw( ec, "Cannot open " + filename.string() );
	}
}
