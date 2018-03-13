#pragma once

#include <vector>
#include "string_tools.h"
#include "xo/container/prop_node.h"

namespace xo
{
	class pattern_matcher
	{
	public:
		pattern_matcher() {}
		pattern_matcher( const prop_node& pn ) : patterns( split_str( pn.get_value(), ";" ) ) {}
		pattern_matcher( const char* pattern, const char* delimeters = ";" ) : patterns( split_str( pattern, delimeters ) ) {}
		pattern_matcher( const string& pattern, const char* delimeters = ";" ) : patterns( split_str( pattern, delimeters ) ) {}

		// returns true if pattern matches string
		bool operator()( const string& str ) const {
			for ( auto& p : patterns )
				if ( pattern_match( str, p ) ) return true;
			return false;
		}

		template< typename T > bool operator()( const T& str ) const { return operator()( static_cast< const string& >( *str ) ); }
		template< typename T > bool operator()( const T* str ) const { return operator()( static_cast< const string& >( *str ) ); }

		template< typename It > It find_match( It begin, It end ) const {
			while ( begin != end && !match( static_cast< const string& >( *begin ) ) ) ++begin;
		}

	private:
		vector< string > patterns;
	};

	IS_PROP_NODE_CONSTRUCTABLE( pattern_matcher );
}

