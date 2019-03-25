#pragma once

#include "xo/string/string_cast.h"
#include <type_traits>

namespace xo
{
	template< typename T, typename E = std::enable_if_t< std::is_enum_v< T > > >
	string to_str( const T& value ) { return to_str( static_cast<int>( value ) ); }

	template< typename T, typename E = std::enable_if_t< std::is_enum_v< T > > >
	bool from_str( const string& s, T& v ) {
		if ( int i; from_str( s, i ) ) {
			v = static_cast<T>( i );
			return true;
		}
		else return false;
	}
}
