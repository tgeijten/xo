#pragma once

#include "xo/xo_types.h"
#include "xo/string/string_type.h"

namespace xo
{
	XO_API void set_debug_output( const path& filename );
	XO_API void write_debug( const string& label, float data );
}
