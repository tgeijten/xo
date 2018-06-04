#pragma once

#include "storage.h"

namespace xo
{
	template< typename T > void write( storage<T>& sto, const string& str, const vec3_<T>& v ) {
		auto f = sto.back();
		f[ str + ".x" ] = v.x;
		f[ str + ".y" ] = v.y;
		f[ str + ".z" ] = v.z;
	}

	template< typename T > void write( storage<T>& sto, const string& str, const quat_<T>& q ) {
		auto f = sto.back();
		f[ str + ".w" ] = q.w;
		f[ str + ".x" ] = q.x;
		f[ str + ".y" ] = q.y;
		f[ str + ".z" ] = q.z;
	}
}
