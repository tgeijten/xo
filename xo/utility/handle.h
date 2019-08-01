#pragma once

#include "xo/system/assert.h"

namespace xo
{
	template< typename T, typename I = uint32_t >
	struct handle
	{
		using value_t = typename I;

		handle() : value( invalid_value() ) {}
		explicit handle( value_t v ) : value( v ) {}
		template< typename U > handle( U v ) : value( value_t( v ) ) { xo_error_if( v >= invalid_value(), "handle cannot hold value" ); }
		handle( const handle& ) = default;
		handle& operator=( const handle& ) = default;
		handle( handle&& o ) : value( o.value ) { o.value = invalid_value(); }
		handle& operator=( handle&& o ) { value_t tmp = value; value = o.value; o.value = tmp; return *this; }

		static constexpr value_t invalid_value() { return ~value_t( 0 ); }

		explicit operator bool() const { return value != invalid_value(); }
		explicit operator value_t() const { return value; }

		friend bool operator==( const handle a, const handle b ) { return a.value == b.value; }
		friend bool operator!=( const handle a, const handle b ) { return a.value != b.value; }

		value_t value;
	};
}
