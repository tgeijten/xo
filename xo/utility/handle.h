#pragma once

#include "xo/system/assert.h"

namespace xo
{
	template< typename ID, typename T = uint32_t >
	struct handle
	{
		handle() : value( invalid_value ) {}
		explicit handle( T v ) : value( v ) {}
		template< typename U > handle( U v ) : value( T( v ) ) { xo_error_if( v >= invalid_value, "handle cannot hold value" ); }
		handle( const handle& ) = default;
		handle& operator=( const handle& ) = default;
		handle( handle&& o ) : value( o.value ) { o.value = invalid_value; }
		handle& operator=( handle&& o ) { T tmp = value; value = o.value; o.value = tmp; return *this; }

		explicit operator bool() const { return value != invalid_value; }
		explicit operator T() const { return value; }

		friend bool operator==( const handle a, const handle b ) { return a.value == b.value; }
		friend bool operator!=( const handle a, const handle b ) { return a.value != b.value; }

		static constexpr T invalid_value = ~T(0);
		T value;
	};
}
