#pragma once

#include "xo/xo_types.h"
#include "xo/string/string_type.h"
#include "xo/string/string_tools.h"
#include "xo/container/prop_node.h"
#include "xo/container/vector_type.h"

namespace xo
{
	class pattern_matcher
	{
	public:
		pattern_matcher() = default;
		pattern_matcher( const prop_node& pn ) : patterns_( split_str( pn.get<string>(), ";" ) ) {}
		pattern_matcher( const char* pattern, const char* delimeters = ";" ) : patterns_( split_str( string( pattern ), delimeters ) ) {}
		pattern_matcher( const string& pattern, const char* delimeters = ";" ) : patterns_( split_str( pattern, delimeters ) ) {}

		// returns true if string matches pattern
		bool match( const string& str ) const {
			for ( auto& p : patterns_ )
				if ( pattern_match( str, p ) )
					return true;
			return false;
		}

		bool operator()( const string& str ) const { return match( str ); }
		bool empty() const { return patterns_.empty(); }
		string str() const { return concat_str( patterns(), ";" );; }
		const vector<string>& patterns() const { return patterns_; }

	private:
		vector<string> patterns_;
	};

	inline bool is_pattern( const string& s ) {
		return s.find_first_of( "*?" ) != string::npos;
	}

	inline bool from_prop_node( const prop_node& pn, pattern_matcher& v ) {
		v = pattern_matcher( pn );
		return true;
	}

	inline string to_str( const pattern_matcher& pm ) {
		return pm.str();
	}
}
