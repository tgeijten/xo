#ifndef XO_ENABLE_PROFILER
#define XO_ENABLE_PROFILER // define to enable global profiler
#endif

#include "test.h"
#include "xo/system/log_sink.h"
#include "xo/diagnose/test_framework.h"
#include "xo/system/system_tools.h"

int main( int argc, char* argv[] )
{
	xo::log::console_sink str( xo::log::info_level );
	xo::log::add_sink( &str );

	int result = xo::test::run_all();

#if XO_IS_DEBUG_BUILD && defined( XO_COMP_MSVC )
	xo::wait_for_key();
#endif

	return result;
}
