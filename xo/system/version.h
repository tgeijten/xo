#pragma once

#include "xo/xo_types.h"
#include "xo/string/string_tools.h"
#include <iosfwd>

namespace xo
{
	struct version
	{
	public:
		version() : major(), minor(), patch(), build(), postfix() {}

		version( int maj, int min, int bugfix, int bld = 0, string post = "" ) :
			major( maj ), minor( min ), patch( bugfix ), build( bld ), postfix( post ) {}

		string str() const {
			string s = stringf( "%d.%d.%d", major, minor, patch );
			if ( build > 0 ) s += stringf( ".%d", build );
			if ( !postfix.empty() ) s += ' ' + postfix;
			return s;
		}

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

	inline std::ostream& operator<<( std::ostream& str, const version& ver ) { str << ver.str(); return str; }
	inline std::istream& operator>>( std::istream& str, version& ver )
	{
		char dummy;
		str >> ver.major >> dummy >> ver.minor >> dummy >> ver.patch >> dummy;
		if ( str.good() ) str >> ver.build;
		if ( str.good() ) str >> ver.postfix;
		return str;
	}
}
