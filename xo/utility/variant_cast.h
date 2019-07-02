#pragma once

#include <variant>

namespace xo
{
	/// convert object from one variant type to another
	template< typename T, typename S >
	T variant_cast( const S& v ) {
		return std::visit( []( auto&& arg ) -> T { return arg; }, v );
	}
}
