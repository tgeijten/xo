#pragma once

#include <ctime>

#include "xo/filesystem/path.h"
#include "xo/system/error_code.h"

namespace xo
{
	XO_API path get_config_folder();
	XO_API path get_documents_folder();
	XO_API path get_application_folder();
	XO_API path temp_directory_path();

	XO_API bool copy_file( const path& from, const path& to, bool overwrite_existing );

	XO_API bool remove( const path& file );

	XO_API bool exists( const path& p );
	XO_API bool file_exists( const path& file );
	XO_API bool folder_exists( const path& folder );

	XO_API bool create_directories( const path& folder );
	XO_API path create_unique_folder( const path& folder, int max_attempts = 9999 );

	XO_API string load_string( const path& filename, error_code* ec = nullptr );

	XO_API void current_path( const path& p );
	XO_API path current_path();

	XO_API std::time_t last_write_time( const path& p );
}
