#pragma once

#ifdef XO_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

#include "xo/xo_types.h"
#include "xo/system/test_case.h"
#include "xo/system/log.h"
#include "xo/system/log_sink.h"

#define XO_TEST_MAIN \
	int main( int argc, char* argv[] ) { \
		xo::log::console_sink sink( xo::log::info_level ); xo::log::add_sink( &sink ); \
		return xo::test::run_all(); }