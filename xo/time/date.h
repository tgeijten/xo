#pragma once

#include "xo/string/string_type.h"
#include "xo/string/string_tools.h"
#include "xo/string/string_cast.h"

namespace xo
{
	struct date {
		int year = 0;
		int month = 0;
		int day = 0;
	};

	inline string to_str( const date& v )
	{
		return stringf( "%04d-%02d-%02d", v.year, v.month, v.day );
	}

	inline bool from_str( const string& s, date& v )
	{
		if ( auto sv = split_str( s, "-./" ); sv.size() == 3 ) {
			bool success = true;
			success &= from_str( sv[ 0 ], v.year );
			success &= from_str( sv[ 1 ], v.month );
			success &= from_str( sv[ 2 ], v.day );
			return success;
		}
		else return false;
	}
}
