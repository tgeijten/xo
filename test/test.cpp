#ifndef XO_ENABLE_PROFILER
#define XO_ENABLE_PROFILER // define to enable global profiler
#endif

#include "test.h"
#include "math_test.h"
#include "buffer_test.h"
#include "xo/container/prop_node.h"
#include "xo/system/system_tools.h"
#include "prop_node_test.h"
#include "xo/time/stopwatch.h"
#include "xo/diagnose/test_framework.h"
#include "string_test.h"
#include "xo/string/dictionary.h"
#include "xo/system/log_sink.h"
#include "factory_test.h"
#include "container_test.h"
#include "timer_test.h"
#include "xo/diagnose/profiler.h"
#include "data_test.h"
#include "delay_test.h"
#include "filter_test.h"
#include "math_test.h"
#include "serializer_test.h"
#include "regression_test.h"
#include "alignment_test.h"
#include "xo/diagnose/debug_tools.h"
#include "xo/numerical/functions.h"
#include "performance_test.h"
#include <variant>
#include "view_if_test.h"

using namespace xo;

int main( int argc, char* argv[] )
{
	log::stream_sink str( log::info_level, std::cout );
	log::add_sink( &str );

	std::variant< float, vec3f, std::string > x;
	auto y = sizeof( x );
	auto z = sizeof( string );

	try
	{
		//xo::performance_test();
		//xo::timer_test();

#ifdef XO_ENABLE_PROFILER
		//xo::profile_test();
#endif

		xo_profiler_reset();
		xo::stopwatch sw;

		xo::serializer_test();
		sw.add_measure( "serializer" );

		xo::container_test();
		sw.add_measure( "container" );

		xo::data_test();
		sw.add_measure( "data" );

		xo::factory_test();
		sw.add_measure( "factory" );

		xo::prop_node_test();
		sw.add_measure( "prop_node" );

		xo::string_test();
		sw.add_measure( "string" );

		xo::buffer_test();
		sw.add_measure( "buffer" );

		xo::regression_test();
		sw.add_measure( "regression" );

		xo::view_if_test();
		xo::optional_test();
		xo::clamp_test();
		xo::linear_regression_test();
		xo::math_test();
		xo::function_test();
		xo::angle_test();
		sw.add_measure( "math" );

		xo::log::info( "Profiler report:\n", xo_profiler_report() );
		xo::log::info( "Test durations:\n", sw.get_report() );
	}
	catch ( std::exception& e )
	{
		std::cout << e.what();				
	}

	int i = xo::test_framework::get_instance().report();

#if XO_IS_DEBUG_BUILD && defined( XO_COMP_MSVC )
	xo::wait_for_key();
#endif

	return i;
}
