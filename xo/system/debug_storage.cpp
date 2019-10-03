#include "debug_storage.h"
#include "xo/container/storage.h"
#include "xo/numerical/constants.h"
#include "xo/filesystem/path.h"
#include <fstream>

namespace xo
{
	constexpr char debug_out_file[] = "debug.txt";

	template< typename T, typename L >
	struct auto_storage_writer
	{
		auto_storage_writer( const path& filename ) : filename_( filename ) {}
		~auto_storage_writer() {
			if ( !storage_.empty() )
				std::ofstream( filename_.str() ) << storage_;
		}

		void write( const L& label, T value ) {
			auto idx = storage_.find_or_add_channel( label, constants<T>::sentinel() );
			if ( storage_.empty() || storage_.back()[ idx ] != constants<T>::sentinel() )
				storage_.add_frame( constants<T>::sentinel() );
			storage_.back()[ idx ] = value;
		}
		path filename_;
		storage< T, L > storage_;
	};

	auto g_debug_storage_writer = auto_storage_writer< float, string >( debug_out_file );

	void set_debug_output( const path& filename )
	{
		g_debug_storage_writer.filename_ = filename;
	}

	void write_debug( const string& label, float data )
	{
		g_debug_storage_writer.write( label, data );
	}
}
