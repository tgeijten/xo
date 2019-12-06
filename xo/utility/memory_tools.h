#pragma once

// #todo: deprecate?

#include <memory>
#include <vector>
#include <string>

namespace xo
{
	// create a new copy of unique_ptr
	template< typename T >
	std::unique_ptr< T > clone( const std::unique_ptr< T >& o )
	{ return std::unique_ptr< T >( new T( *o ) ); }

	// create a new copy of vector< unique_ptr >
	template< typename T >
	std::vector< std::unique_ptr< T > > clone( const std::vector< std::unique_ptr< T > >& o ) {
		std::vector< std::unique_ptr< T > > r;
		r.reserve( o.size() );
		for ( auto& e : o ) r.push_back( clone( e ) );
		return r;
	}

	template< typename T >
	T& dereference_or_throw( T* v, const std::string& message ) {
		xo_error_if( !v, message );
		return *v;
	}

	template< typename T >
	const T& dereference_or_throw( const T* v, const std::string& message ) {
		xo_error_if( !v, message );
		return *v;
	}
}
