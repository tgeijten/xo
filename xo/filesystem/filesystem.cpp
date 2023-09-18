#include "filesystem.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef XO_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <conio.h>
#	include <shlobj.h>
#   include <direct.h>
#	pragma warning( disable: 4996 )
#else
#   include <unistd.h>
#endif

#ifdef __APPLE__
#	include <mach-o/dyld.h>
#endif

#include "xo/string/string_tools.h"
#include "xo/system/assert.h"
#include "xo/container/container_tools.h"
#include "xo/string/string_cast.h"
#include "xo/system/log.h"

namespace fs = std::filesystem;

namespace xo
{
	// thread local path used by find_file()
	thread_local path g_current_find_file_folder = path();

#ifdef XO_COMP_MSVC
	// define this helper function for windows, since it's so complicated
	path get_known_windows_folder( KNOWNFOLDERID id, const char* report_name )
	{
		// get the string, convert to single byte string, then free the original string (ugh)
		wchar_t* wcsLocalAppData = 0;
		if ( SHGetKnownFolderPath( id, 0, NULL, &wcsLocalAppData ) != S_OK ) {
			log::error( "Could get folder ", report_name );
			return path();
		}
		char mbsLocalAppData[MAX_PATH];
		if ( wcstombs_s( size_t(), mbsLocalAppData, MAX_PATH, wcsLocalAppData, MAX_PATH ) != 0 ) {
			log::error( "Could not convert folder ", report_name, ": ", wcsLocalAppData );
			CoTaskMemFree( static_cast<void*>( wcsLocalAppData ) );
			return path();
		}

		CoTaskMemFree( static_cast<void*>( wcsLocalAppData ) );
		return path( mbsLocalAppData ).make_preferred();
	}
#endif

	path get_config_dir()
	{
#ifdef XO_COMP_MSVC
		return get_known_windows_folder( FOLDERID_LocalAppData, "LocalAppData" );
#else
		string homeDir = std::getenv( "HOME" );
		return path{ homeDir + "/.config" };
#endif
	}

	path get_documents_dir()
	{
#ifdef XO_COMP_MSVC
		return get_known_windows_folder( FOLDERID_Documents, "Documents" );
#elif defined(__APPLE__)
		return path( std::getenv( "HOME" ) ) / "Documents";
#else
		return path( std::getenv( "HOME" ) );
#endif
	}

	path get_application_dir()
	{
#ifdef XO_COMP_MSVC
		char buf[1024];
		GetModuleFileName( 0, buf, sizeof( buf ) );
		return path( buf ).parent_path().make_preferred();
#elif defined(__linux__)
		char buf[1024];
		auto len = readlink( "/proc/self/exe", buf, sizeof( buf ) - 1 );
		if ( len != -1 ) {
			buf[len] = '\0';
			return path( buf ).parent_path();
		}
		else return path();
#elif defined(__APPLE__)
		uint32_t bufferSize = 1024;
		std::vector<char> buf( bufferSize + 1 );
		if ( _NSGetExecutablePath( buf.data(), &bufferSize ) )
		{
			buf.resize( bufferSize );
			_NSGetExecutablePath( buf.data(), &bufferSize );
		}
		return path( buf.data() ).parent_path();
#else
		return path();
#endif
	}

	path temp_directory_path()
	{
		if ( auto* sz = std::getenv( "TMP" ) )
			return path( sz );
		else return path( "/tmp" );
	}

	bool copy_file( const path& from, const path& to, bool overwrite, error_code* ec )
	{
		auto opt = overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::none;
		if ( ec ) {
			std::error_code stdec;
			auto success = fs::copy_file( fs::path( from.str() ), fs::path( to.str() ), opt, stdec );
			if ( !success )
				ec->set( stdec.value(), stdec.message() );
			return success;
		}
		else return fs::copy_file( fs::path( from.str() ), fs::path( to.str() ), opt );
	}

	bool remove( const path& file )
	{
		return std::remove( file.c_str() ) == 0;
	}

	bool exists( const path& p )
	{
#ifdef XO_COMP_MSVC
		return GetFileAttributes( path( p ).make_preferred().c_str() ) != INVALID_FILE_ATTRIBUTES;
#else
		struct stat info;
		return stat( p.c_str(), &info ) == 0;
#endif
	}

	bool file_exists( const path& file )
	{
		std::ifstream ifs( file.str() );
		return ifs.good();
	}

	bool directory_exists( const path& folder )
	{
#ifdef XO_COMP_MSVC
		DWORD dwAttrib = GetFileAttributes( path( folder ).make_preferred().c_str() );
		return ( dwAttrib != INVALID_FILE_ATTRIBUTES && ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
#else
		struct stat status;
		if ( stat( folder.c_str(), &status ) == 0 )
		{
			if ( status.st_mode & S_IFDIR )
				return true;
		}
		return false; // if any condition fails
#endif
	}

	void current_find_file_path( const path& folder )
	{
		g_current_find_file_folder = folder;
	}

	const path& current_find_file_path()
	{
		return g_current_find_file_folder;
	}

	path find_file( std::initializer_list< path > find_paths )
	{
		for ( auto& f : find_paths )
		{
			if ( f.is_absolute() && file_exists( f ) )
				return f;
			else if ( auto full_path = current_find_file_path() / f; file_exists( full_path ) )
				return full_path;
		}
		xo_error( "Could not find any of the following files: " + container_to_str( find_paths ) );
	}

	optional< path > try_find_file( std::initializer_list< path > find_paths )
	{
		for ( auto& f : find_paths )
		{
			if ( f.is_absolute() ) {
				if ( file_exists( f ) )
					return f;
			}
			else {
				auto full_path = current_find_file_path() / f;
				if ( file_exists( full_path ) )
					return full_path;
			}
		}
		return optional< path >();
	}

	bool create_directories( const path& folder )
	{
		// make sure parent folders exist
		if ( folder.has_parent_path() && !directory_exists( folder.parent_path() ) )
			create_directories( folder.parent_path() );

#ifdef XO_COMP_MSVC
		return CreateDirectory( folder.c_str(), NULL ) != 0;
#else
		return mkdir( folder.c_str(), 0777 ) == 0;
#endif
	}

	path find_unique_directory( const path& folder, int max_attempts )
	{
		path unique_folder = folder;
		for ( int i = 0; i < max_attempts; ++i )
		{
			if ( i > 0 )
				unique_folder = folder + stringf( " (%d)", i );
			if ( !directory_exists( unique_folder ) )
				return unique_folder;
		}
		xo_error( "Could not find unique folder after " + to_str( max_attempts ) + " attempts" );
	}

	path create_unique_directory( const path& folder, int max_attempts )
	{
		path unique_folder = folder;
		bool success = false;
		for ( int i = 0; i < max_attempts && !success; ++i )
		{
			if ( i > 0 )
				unique_folder = folder + stringf( " (%d)", i );

			if ( !directory_exists( unique_folder ) )
				success = create_directories( unique_folder ); // try to create folder
		}
		xo_error_if( !success, "Could not create unique folder based on " + folder.str() + " after " + to_str( max_attempts ) + " attempts" );

		return unique_folder;
	}

	string load_string( const path& filename, error_code* ec )
	{
		// this method uses a stringbuf, which requires an extra copy (C++ suckiness)
		std::ifstream ifstr( filename.str() );
		if ( !ifstr.good() )
			return set_error_or_throw( ec, "Could not open " + filename.str() ), "";

		std::stringstream str;
		str << ifstr.rdbuf();
		return str.str();
	}

	void save_string( const path& filename, const string& s, error_code* ec )
	{
		xo::create_directories( filename.parent_path().str() );
		std::ofstream ofstr( filename.str() );
		if ( ofstr.good() )
			ofstr << s;
		else set_error_or_throw( ec, "Could not open " + filename.str() );
	}

	void append_string( const path& filename, const string& s, error_code* ec )
	{
		xo::create_directories( filename.parent_path().str() );
		std::ofstream ofstr( filename.str(), std::ios_base::app );
		if ( ofstr.good() )
			ofstr << s;
		else set_error_or_throw( ec, "Could not open " + filename.str() );
	}

	bool current_path( const path& p )
	{
		return chdir( p.c_str() ) == 0;
	}

	path current_path()
	{
		char* buf = getcwd( NULL, 0 );
		path p( buf );
		free( buf );
		return p;
	}

	std::time_t last_write_time( const path& p )
	{
#ifdef XO_COMP_MSVC
#	ifdef _USE_32BIT_TIME_T
		struct _stat32 result;
		if ( _stat32( p.c_str(), &result ) == 0 )
			return result.st_mtime;
		else xo_error( "Could not query " + p.str() );
#	else
		struct _stat64 result;
		if ( _stat64( p.c_str(), &result ) == 0 )
			return result.st_mtime;
		else xo_error( "Could not query " + p.str() );
#	endif

#else
		struct stat attr;
		if ( stat( p.c_str(), &attr ) == 0 )
			return attr.st_mtime;
		else
			xo_error( "Could not query " + p.str() );
#endif
	}

	vector<path> find_files( const path& dir, const pattern_matcher& include, bool recursive, int recurse_levels )
	{
		vector<path> files;
		for ( fs::directory_iterator it( dir.str() ); it != fs::directory_iterator(); ++it )
		{
			const auto& fs_path = it->path();
			if ( recursive && recurse_levels != 0 && fs::is_directory( fs_path ) )
				xo::append( files, find_files( path( fs_path.string() ), include, recursive, recurse_levels - 1 ) );
			if ( include( fs_path.filename().string() ) )
				files.emplace_back( fs_path.string() );
		}
		return files;
	}

	vector<path> find_files( const path& dir, const pattern_matcher& include, const pattern_matcher& exclude, bool recursive, int recurse_levels )
	{
		vector<path> files;
		for ( fs::directory_iterator it( dir.str() ); it != fs::directory_iterator(); ++it )
		{
			const auto& fs_path = it->path();
			if ( recursive && recurse_levels != 0 && fs::is_directory( fs_path ) )
				xo::append( files, find_files( path( fs_path.string() ), include, exclude, recursive, recurse_levels - 1 ) );
			if ( include( fs_path.filename().string() ) && !exclude( fs_path.filename().string() ) )
				files.emplace_back( fs_path.string() );
		}
		return files;
	}
}
