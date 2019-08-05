#pragma once

#include "xo/xo_types.h"
#include "xo/system/log.h"
#include "xo/system/log_sink.h"
#include <string>
#include <vector>

#define XO_TEST_CASE( _name_ ) \
	static void _name_( ::xo::test::test_case& ); \
	auto _name_##_case = ::xo::test::test_case( #_name_, &_name_ ); \
	static void _name_( ::xo::test::test_case& XO_ACTIVE_TEST_CASE )

#define XO_TEST_CASE_SKIP( _name_ ) \
	static void _name_( ::xo::test::test_case& XO_ACTIVE_TEST_CASE )

#define XO_CHECK( _operation_ ) \
	try { bool _result_ = ( _operation_ ); XO_ACTIVE_TEST_CASE.check( _result_, #_operation_ ); } \
	catch( std::exception& e ) { XO_ACTIVE_TEST_CASE.check( false, #_operation_, e.what() ); }

#define XO_CHECK_MESSAGE( _operation_, _message_ ) \
	try { bool _result_ = ( _operation_ ); XO_ACTIVE_TEST_CASE.check( _result_, #_operation_, _message_ ); } \
	catch( std::exception& e ) { XO_ACTIVE_TEST_CASE.check( false, #_operation_, e.what() ); }

#define XO_TEST_MAIN \
	int main( int argc, char* argv[] ) { \
		xo::log::console_sink sink( xo::log::info_level ); xo::log::add_sink( &sink ); \
		return xo::test::run_all(); }

namespace xo
{
	namespace test
	{
		class test_case;
		XO_API void register_test_case( test_case* tc );
		XO_API std::vector< test_case* >& get_test_cases();
		XO_API int run_all();

		typedef void( *test_func_t )( test_case& );
		class XO_API test_case
		{
		public:
			test_case( const char* name, test_func_t func );
			bool check( bool result, const char* operation, const std::string& message = "" );
			int run( int* checks = nullptr, int* passed = nullptr, int* failed = nullptr );

		private:
			test_func_t func_;
			const char* name_;
			int num_checks_;
			int num_passed_;
			int num_failed_;
		};
	}
}
