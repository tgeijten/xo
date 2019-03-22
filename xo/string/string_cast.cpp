#include "string_cast.h"

namespace xo
{
	string to_str( float value )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", value );
		return string( buf );
	}

	string to_str( double value )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", value );
		return string( buf );
	}

	string to_str( bool value )
	{
		return string( value ? "1" : "0" );
	}

	string to_str( int value )
	{
		return std::to_string( value );
	}

	string to_str( unsigned int value )
	{
		return std::to_string( value );
	}

	string to_str( long value )
	{
		return std::to_string( value );
	}

	string to_str( unsigned long value )
	{
		return std::to_string( value );
	}

	string to_str( const char* value )
	{
		return string( value );
	}
}
