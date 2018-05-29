#include "test_framework.h"
#include "xo/system/log.h"

namespace xo
{
	xo::test_framework test_framework::test_;

	test_framework::test_framework() :
	num_tests( 0 ),
	num_passed( 0 ),
	num_failed( 0 )
	{}

	bool test_framework::test( const char* name, bool result, const std::string& message )
	{
		++num_tests;
		if ( result )
		{
			if ( show_passed )
				log::info( "TEST ", num_tests, " ", name, ": passed. ", message );

			num_passed++;
			return true;
		}
		else
		{
			log::error( "TEST ", num_tests, " ", name, ": FAILED! ", message );
			num_failed++;
			return false;
		}
	}

	void test_framework::reset()
	{
		num_tests = num_passed = num_failed;
	}

	int test_framework::report()
	{
		auto level = num_failed > 0 ? log::error_level : log::info_level;
		log::message( level, "Test results: ", num_tests, " performed, ", num_failed, " failed, ", num_passed, " passed" );

		return num_failed > 0 ? -1 : 0;
	}

	xo::test_framework& test_framework::get_instance()
	{
		return test_;
	}

}
