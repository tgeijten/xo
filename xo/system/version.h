#pragma once

#include "xo/system/xo_api.h"
#include "xo/xo_types.h"
#include "xo/string/string_type.h"

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

	XO_API string to_str( const version& v );
	XO_API bool from_str( const string& s, version& ver );
}
