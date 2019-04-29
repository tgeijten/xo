#include "prop_node_serializer.h"

#include <fstream>

#include "xo/system/error_code.h"
#include "xo/container/prop_node.h"

namespace xo
{
	prop_node_serializer::prop_node_serializer() :
		read_pn_( nullptr ), write_pn_( nullptr ), ec_( nullptr ), file_folder_()
	{}

	prop_node_serializer::prop_node_serializer( const prop_node& pn, error_code* ec, const path& file_folder ) :
		read_pn_( nullptr ), write_pn_( &pn ), ec_( ec ), file_folder_( file_folder )
	{}

	prop_node_serializer::prop_node_serializer( prop_node& pn, error_code* ec, const path& file_folder ) :
		read_pn_( &pn ), write_pn_( &pn ), ec_( ec ), file_folder_( file_folder )
	{}

	prop_node prop_node_serializer::load_file( const path& filename, error_code* ec )
	{
		std::ifstream str( filename.str() );
		if ( str )
		{
			prop_node pn;
			read_pn_ = &pn;
			ec_ = ec;
			file_folder_ = filename.parent_path();
			read_stream( str );
			return *read_pn_;
		}
		else return set_error_or_throw( ec, "Cannot open " + filename.str() ), prop_node();
	}

	void prop_node_serializer::save_file( const prop_node& pn, const path& filename, error_code* ec )
	{
		std::ofstream str( filename.str() );
		if ( str )
		{
			write_pn_ = &pn;
			ec_ = ec;
			file_folder_ = filename.parent_path();
			write_stream( str );
		}
		else set_error_or_throw( ec, "Cannot open " + filename.str() );
	}
}
