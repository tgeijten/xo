#pragma once

#include "xo/system/platform.h"
#include "xo/filesystem/path.h"

namespace xo
{
	XO_API void set_debug_output( const path& filename );
	XO_API void write_debug( const string& label, float data );
}
