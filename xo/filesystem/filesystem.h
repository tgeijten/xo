#pragma once

#include <ctime>

#include "xo/filesystem/path.h"
#include "xo/system/error_code.h"
#include "xo/utility/optional.h"
#include "xo/string/pattern_matcher.h"
#include "xo/container/vector_type.h"

namespace xo
{
	XO_API path get_config_dir();
	XO_API path get_documents_dir();
	XO_API path get_application_dir();
	XO_API path temp_directory_path();

	XO_API bool copy_file( const path& from, const path& to, bool overwrite_existing, error_code* ec = nullptr );

	XO_API bool remove( const path& file );

	XO_API bool exists( const path& p );
	XO_API bool file_exists( const path& file );
	XO_API bool directory_exists( const path& folder );

	XO_API void current_find_file_path( const path& dir );
	XO_API const path& current_find_file_path();

	XO_API path find_file( std::initializer_list< path > paths );
	XO_API optional<path> try_find_file( std::initializer_list< path > paths );

	XO_API bool create_directories( const path& folder );
	XO_API path find_unique_directory( const path& folder, int max_attempts = 9999 );
	XO_API path create_unique_directory( const path& folder, int max_attempts = 9999 );

	XO_API string load_string( const path& filename, error_code* ec = nullptr );
	XO_API void save_string( const path& filename, const string& s, error_code* ec = nullptr  );
	XO_API void append_string( const path& filename, const string& s, error_code* ec = nullptr  );

	XO_API bool current_path( const path& p );
	XO_API path current_path();

	XO_API std::time_t last_write_time( const path& p );

	XO_API vector<path> find_files( const path& dir, const pattern_matcher& include, bool recursive = true, int recurse_levels = -1 );
	XO_API vector<path> find_files( const path& dir, const pattern_matcher& include, const pattern_matcher& exclude, bool recursive = true, int recurse_levels = -1 );
}
