#pragma once

#include "xo/xo_types.h"
#include "xo/container/vector_type.h"
#include "xo/utility/frange.h"
#include <functional>

namespace xo
{
	template< typename T >
	vector<T> evaluate_function( std::function<T(T)> f, const frange<T>& range ) {
		vector<T> v;
		v.reserve( size_t( range.max_step_ ) );
		for ( auto x : range )
			v.push_back( f( x ) );
		return v;
	}

	template< typename T >
	vector<T> evaluate_function( std::function<T(T)> f, T begin, T end, T interval ) {
		return evaluate_function( f, frange( begin, end, interval ) );
	}
}
