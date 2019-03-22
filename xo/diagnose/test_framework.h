#pragma once

#include "xo/system/xo_api.h"
#include "xo/xo_types.h"
#include <string>

#define XO_TEST( operation ) { try { xo::test_framework::get_instance().test( #operation, operation ); } catch( const std::exception& e ) { xo::test_framework::get_instance().test( #operation, false, e.what() ); } }
#define XO_TEST_MSG( operation, message ) { try { xo::test_framework::get_instance().test( #operation, operation, message ); } catch( const std::exception& e ) { xo::test_framework::get_instance().test( #operation, false, e.what() ); } }
#define XO_TEST_REPORT xo::test_framework::get_instance().report

namespace xo
{
	class XO_API test_framework
	{
	public:
		test_framework();
		bool test( const char* name, bool result, const std::string& message = "" );
		void reset();
		int report();

		int num_tests;
		int num_failed;
		int num_passed;

		bool show_passed = false;

		static test_framework& get_instance();
		static test_framework test_;
	};
}
