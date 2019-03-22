#include "string_cast.h"

namespace xo
{
	string to_str( const float& value )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", value );
		return string( buf );
	}

	string to_str( const double& value )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", value );
		return string( buf );
	}

	string to_str( const bool& value )
	{
		return string( value ? "1" : "0" );
	}

	string to_str( const unsigned int& value )
	{
		return std::to_string( value );
	}

	string to_str( const long& value )
	{
		return std::to_string( value );
	}

	string to_str( const unsigned long& value )
	{
		return std::to_string( value );
	}

	string to_str( const char* value )
	{
		return string( value );
	}
}
