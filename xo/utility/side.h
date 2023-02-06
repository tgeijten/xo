#pragma once

#include "xo/string/string_type.h"
#include "xo/string/string_tools.h"

namespace xo
{
	enum class side {
		left = -1,
		none = 0,
		right = 1
	};

	inline const char* side_postfix( side s ) {
		static constexpr const char* side_postfixes[3] = { "_l", "", "_r" };
		return side_postfixes[static_cast<int>( s ) + 1];
	}

	inline side str_get_side( const string& str ) {
		if ( str_ends_with( str, "_l" ) )
			return side::left;
		else if ( str_ends_with( str, "_r" ) )
			return side::right;
		else return side::none;
	}

	inline string str_remove_side( const string& str ) {
		if ( str_get_side( str ) != side::none )
			return left_str( str, -2 );
		else return str;
	}

	inline string str_set_side( const string& str, side new_side ) {
		auto cur_side = str_get_side( str );
		if ( cur_side == new_side )
			return str;
		else if ( cur_side == side::none )
			return str + side_postfix( new_side );
		else if ( new_side == side::none )
			return str_remove_side( str );
		else {
			auto new_str = string( str );
			new_str.back() = ( new_side == side::left ) ? 'l' : 'r';
			return new_str;
		}
	}
}
