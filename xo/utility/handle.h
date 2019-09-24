#pragma once

#include "xo/system/assert.h"
#include "xo/xo_types.h"

namespace xo
{
	template< typename T, typename I = uint32 >
	struct handle
	{
		using id_type = I;

		handle() : value( invalid_value() ) {}
		explicit handle( id_type v ) : value( v ) {}
		template< typename U > handle( U v ) : value( id_type( v ) ) { xo_error_if( v >= invalid_value(), "handle cannot hold value" ); }
		handle( const handle& ) = default;
		handle& operator=( const handle& ) = default;
		handle( handle&& o ) noexcept : value( o.value ) { o.value = invalid_value(); }
		handle& operator=( handle&& o ) noexcept { id_type tmp = value; value = o.value; o.value = tmp; return *this; }

		static constexpr id_type invalid_value() { return ~id_type( 0 ); }

		explicit operator bool() const { return value != invalid_value(); }
		explicit operator id_type() const { return value; }

		friend bool operator==( const handle a, const handle b ) { return a.value == b.value; }
		friend bool operator!=( const handle a, const handle b ) { return a.value != b.value; }

		id_type value;
	};
}
