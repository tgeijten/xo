#pragma once

#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include "xo/string/string_tools.h"
#include "xo/container/prop_node.h"
#include <vector>

namespace xo
{
	class XO_API pattern_matcher
	{
	public:
		pattern_matcher() {}
		pattern_matcher( const prop_node& pn ) :
			patterns( split_str( pn.get<string>(), ";" ) )
		{}
		pattern_matcher( const char* pattern, const char* delimeters = ";" ) :
			patterns( split_str( pattern, delimeters ) )
		{}
		pattern_matcher( const string& pattern, const char* delimeters = ";" ) :
			patterns( split_str( pattern, delimeters ) )
		{}

		// returns true if string matches pattern
		bool operator()( const string& str ) const {
			for ( auto& p : patterns )
				if ( pattern_match( str, p ) )
					return true;
			return false;
		}

	private:
		std::vector< string > patterns;
	};

	inline bool is_pattern( const string& s )
	{
		return s.find_first_of( "*?" ) != string::npos;
	}
	inline bool from_prop_node( const prop_node& pn, pattern_matcher& v )
	{
		v = pattern_matcher( pn );
		return true;
	}
}
