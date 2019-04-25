#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"

namespace xo
{
	struct version
	{
	public:
		version() : majorVer(), minorVer(), patch(), build(), postfix() {}

		version( int maj, int min, int bugfix, int bld = 0, string post = "" ) :
			majorVer( maj ), minorVer( min ), patch( bugfix ), build( bld ), postfix( post ) {}

		int to_int100() const { return majorVer * 10000 + minorVer * 100 + patch; }
		bool empty() const { return majorVer == 0 && minorVer == 0 && patch == 0 && build == 0 && postfix.empty(); }

		int majorVer;
		int minorVer;
		int patch;
		int build;
		string postfix;
	};

	inline bool operator<( const version& v1, const version& v2 ) { return v1.to_int100() < v2.to_int100(); }
	inline bool operator==( const version& v1, const version& v2 ) { return v1.to_int100() == v2.to_int100(); }
	inline bool operator!=( const version& v1, const version& v2 ) { return v1.to_int100() != v2.to_int100(); }

	XO_API string to_str( const version& v );
	XO_API bool from_str( const string& s, version& ver );
}
