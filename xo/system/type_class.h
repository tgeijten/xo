#pragma once

#include <type_traits>

namespace xo
{
	enum type_class { unknown_type_class, void_type_class, boolean_type_class, integral_type_class, floating_point_type_class, enum_type_class, string_type_class, };

	template< typename T >type_class get_type_class() {
		if ( std::is_void< T >::value )
			return void_type_class;
		else if ( std::is_same< T, bool >::value )
			return boolean_type_class;
		else if ( std::is_integral< T >::value )
			return integral_type_class;
		else if ( std::is_floating_point< T >::value )
			return floating_point_type_class;
		else if ( std::is_enum< T >::value )
			return enum_type_class;
		else if ( std::is_same< T, std::string >::value || std::is_same< T, char* >::value )
			return string_type_class;
		else return unknown_type_class;
	}
}
