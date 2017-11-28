#pragma once

#define xo_result_type( A_, B_ ) decltype( A_(1) + B(1) )

namespace xo
{
	/// result types
	template< typename A, typename B > struct result_type { typedef decltype( A(1) + B(1) ) type; };
}
