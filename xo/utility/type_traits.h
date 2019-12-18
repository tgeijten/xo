#pragma once

namespace xo
{
	template< class T > struct remove_const { typedef T type; };
	template< class T > struct remove_const<const T> { typedef T type; };

	template< class T > struct remove_volatile { typedef T type; };
	template< class T > struct remove_volatile<volatile T> { typedef T type; };
}
