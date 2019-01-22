#pragma once

#include <fstream>
#include <functional>
#include "xo/filesystem/path.h"
#include <iomanip>

namespace xo
{
	template< typename T >
	void write_function( const path& file, T min, T max, T ( *func )( T ), int samples = 101 )
	{
		std::ofstream of( file.c_str() );
		of << std::setprecision( 6 );
		for ( int i = 0; i < samples; ++i )
		{
			T x = min + i * ( max - min ) / ( samples - 1 );
			T y = func( x );
			of << x << '\t' << y << std::endl;
		}
	}
}
