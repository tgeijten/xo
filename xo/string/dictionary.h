#pragma once

#include "xo/xo_types.h"
#include "xo/system/assert.h"
#include "xo/container/container_tools.h"
#include "xo/string/string_cast_enum.h"

namespace xo
{
	template< typename T >
	class dictionary
	{
	public:
		using pair_t = std::pair< T, string >;
		dictionary( std::initializer_list< pair_t > l ) : data( l ) {}

		/// lookup name of element e
		const string& operator()( const T& e ) const {
			auto it = find_if( data, [&]( const pair_t& p ) { return e == p.first; } );
			xo_error_if( it == data.end(), "Could not find element " + to_str( e ) );
			return it->second;
		}

		/// lookup element with name str
		const T& operator()( const string& str ) const {
			auto it = find_if( data, [&]( const pair_t& p ) { return str == p.second; } );
			xo_error_if( it == data.end(), "Could not find element with name " + str );
			return it->first;
		}

		/// lookup element with name str or return default
		const T& operator()( const string& str, const T& value_not_found ) const {
			auto it = find_if( data, [&]( const pair_t& p ) { return str == p.second; } );
			return ( it == data.end() ) ? value_not_found : it->first;
		}

	private:
		std::vector< pair_t > data;
	};

	template< typename T >
	dictionary< T > make_dictionary( std::initializer_list< std::pair< T, string > > l ) { return dictionary< T >( l ); }

	template< typename T >
	T lookup( const string& value, std::initializer_list< std::pair< const char*, T > > table )
	{
		for ( auto& entry : table )
			if ( value == entry.first )
				return entry.second;
		xo_error( "Unexpected value: " + value );
	}
}
