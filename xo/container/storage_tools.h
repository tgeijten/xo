#pragma once

#include "storage.h"

namespace xo
{
	template< typename T > void write( storage<T>& sto, const string& str, const vec3_<T>& v ) {
		sto[ str + ".x" ] = v.x;
		sto[ str + ".y" ] = v.y;
		sto[ str + ".z" ] = v.z;
	}

	template< typename T > void write( storage<T>& sto, const string& str, const quat_<T>& q ) {
		sto[ str + ".w" ] = q.w;
		sto[ str + ".x" ] = q.x;
		sto[ str + ".y" ] = q.y;
		sto[ str + ".z" ] = q.z;
	}
}
