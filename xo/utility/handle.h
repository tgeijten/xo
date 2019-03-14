#pragma once

#include "xo/string/string_tools.h"

namespace xo
{
	template< typename ID, typename T = uint32_t >
	struct handle
	{
		constexpr T invalid_v = T( -1 );

		handle() : value( invalid_v ) {}
		explicit handle( T v ) : value( v ) {}
		template< typename U > handle( U v ) : value( T( v ) ) { xo_error_if( v < constants<T>::lowest() || v >= constants<T>::max(), "handle cannot hold value " + to_str( v ) ); }
		handle( const handle& o ) = delete;
		handle& operator=( const handle& o ) = delete;
		handle( handle&& o ) { value = o.value; o.value = invalid_v; }
		handle& operator=( handle&& o ) { value = o.value; o.value = invalid_v; return *this; }

		explicit operator bool() const { return value != T( -1 ); }
		explicit operator T() const { return value; }

		friend bool operator==( const handle a, const handle b ) { return a.value == b.value; }
		friend bool operator!=( const handle a, const handle b ) { return a.value != b.value; }

		T value;
	};

	template< typename ID, typename T >
	inline std::ostream& operator<<( std::ostream& str, const handle< ID, T >& h ) { return str << h.value; }
}
