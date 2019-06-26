#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"

namespace xo
{
	struct version
	{
		version() : major_(), minor_(), patch_(), build_(), postfix_() {}
		version( int maj, int min, int bugfix, int bld = 0, string post = "" ) :
			major_( maj ), minor_( min ), patch_( bugfix ), build_( bld ), postfix_( post ) {}

		int to_int100() const { return major_ * 10000 + minor_ * 100 + patch_; }
		bool empty() const { return major_ == 0 && minor_ == 0 && patch_ == 0 && build_ == 0 && postfix_.empty(); }

		int major_;
		int minor_;
		int patch_;
		int build_;
		string postfix_;
	};

	inline bool operator<( const version& v1, const version& v2 ) { return v1.to_int100() < v2.to_int100(); }
	inline bool operator==( const version& v1, const version& v2 ) { return v1.to_int100() == v2.to_int100(); }
	inline bool operator!=( const version& v1, const version& v2 ) { return v1.to_int100() != v2.to_int100(); }
}
