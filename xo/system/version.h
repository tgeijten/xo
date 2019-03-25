#pragma once

#include "xo/xo_types.h"
#include "xo/string/string_cast.h"
#include "xo/string/string_tools.h"

namespace xo
{
	struct version
	{
	public:
		version() : major(), minor(), patch(), build(), postfix() {}

		version( int maj, int min, int bugfix, int bld = 0, string post = "" ) :
			major( maj ), minor( min ), patch( bugfix ), build( bld ), postfix( post ) {}

		int to_int100() const { return major * 10000 + minor * 100 + patch; }
		bool empty() const { return major == 0 && minor == 0 && patch == 0 && build == 0 && postfix.empty(); }

		int major;
		int minor;
		int patch;
		int build;
		string postfix;
	};

	inline bool operator<( const version& v1, const version& v2 ) { return v1.to_int100() < v2.to_int100(); }
	inline bool operator==( const version& v1, const version& v2 ) { return v1.to_int100() == v2.to_int100(); }
	inline bool operator!=( const version& v1, const version& v2 ) { return v1.to_int100() != v2.to_int100(); }

	inline string to_str( const version& v )
	{
		string s = stringf( "%d.%d.%d", v.major, v.minor, v.patch );
		if ( v.build > 0 ) s += stringf( ".%d", v.build );
		if ( !v.postfix.empty() ) s += ' ' + v.postfix;
		return s;

	}

	inline version from_str( const string& s, const version& def )
	{
		if ( auto vs = split_str( s, ". " ); vs.size() > 3 )
		{
			version ver;
			ver.major = from_str( vs[ 0 ], ver.major );
			ver.minor = from_str( vs[ 1 ], ver.minor );
			ver.patch = from_str( vs[ 2 ], ver.patch );
			if ( vs.size() >= 3 )
				ver.build = from_str( vs[ 3 ], ver.build );
			if ( vs.size() >= 4 )
				ver.postfix = vs[ 4 ]; // #TODO: this method is incorrect when postfix contains delimiters
			return ver;
		}
		else return def;
	}
}
