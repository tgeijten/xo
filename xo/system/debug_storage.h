#pragma once

#include "xo/xo_types.h"
#include "xo/filesystem/path.h"
#include "xo/string/string_type.h"
#include <fstream>
#include <iostream>
#include <iomanip>

namespace xo
{
	XO_API void set_debug_output( const path& filename );
	XO_API void write_debug( const string& label, float data );

	template< typename T >
	void write_function( const path& file, T min, T max, simple_function_t<T> func, int samples = 101 ) {
		std::ofstream of( file.c_str() );
		of << std::setprecision( 6 );
		for ( int i = 0; i < samples; ++i )	{
			T x = min + i * ( max - min ) / ( samples - 1 );
			T y = func( x );
			of << x << '\t' << y << std::endl;
		}
	}
}
