#pragma once

#include "xo/xo_types.h"
#include "xo/filesystem/path.h"
#include "xo/filesystem/filesystem.h"
#include "xo/string/string_type.h"
#include "xo/numerical/compare.h"
#include <fstream>
#include <iostream>
#include <iomanip>

namespace xo
{
	XO_API void set_debug_output( const path& filename );
	XO_API void write_debug( const string& label, float data );

	template< typename T >
	void write_function( const path& file, T min, T max, simple_function_t<T> func, T interval = T( 0.01 ) ) {
		xo::create_directories( file.parent_path() );
		std::ofstream of( file.c_str() );
		xo_error_if( !of.good(), "Could not create " + file.str() );
		of << std::setprecision( 6 );
		for ( T x = min; less_than_or_equal( x, max ); x += interval ) {
			T y = func( x );
			of << x << '\t' << y << std::endl;
		}
	}
}
