#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"

namespace xo
{
	struct version
	{
		version() : major_(), minor_(), patch_(), build_(), postfix_() {}
		version( int maj, int min, int patch, int bld = 0, string post = "" ) :
			major_( maj ), minor_( min ), patch_( patch ), build_( bld ), postfix_( post ) {}

		bool empty() const { return major_ == 0 && minor_ == 0 && patch_ == 0 && build_ == 0 && postfix_.empty(); }

		int major_;
		int minor_;
		int patch_;
		int build_;
		string postfix_;
	};

	inline int version_compare( const version& v1, const version& v2 ) {
		if ( v1.major_ < v2.major_ )
			return -1;
		else if ( v1.major_ > v2.major_ )
			return 1;
		else if ( v1.minor_ < v2.minor_ )
			return -2;
		else if ( v1.minor_ > v2.minor_ )
			return 2;
		else if ( v1.patch_ < v2.patch_ )
			return -3;
		else if ( v1.patch_ > v2.patch_ )
			return 3;
		else if ( v1.build_ < v2.build_ )
			return -4;
		else if ( v1.build_ > v2.build_ )
			return 4;
		else return 0;
	}

	inline bool operator<( const version& v1, const version& v2 ) { return version_compare( v1, v2 ) < 0; }
	inline bool operator>( const version& v1, const version& v2 ) { return version_compare( v1, v2 ) > 0; }
	inline bool operator<=( const version& v1, const version& v2 ) { return version_compare( v1, v2 ) <= 0; }
	inline bool operator>=( const version& v1, const version& v2 ) { return version_compare( v1, v2 ) >= 0; }
	inline bool operator==( const version& v1, const version& v2 ) { return version_compare( v1, v2 ) == 0; }
	inline bool operator!=( const version& v1, const version& v2 ) { return version_compare( v1, v2 ) != 0; }
}
