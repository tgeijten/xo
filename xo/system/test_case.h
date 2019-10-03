#pragma once

#include "xo/xo_types.h"
#include "xo/system/assert.h"
#include "xo/string/string_type.h"
#include "xo/container/vector_type.h"

#define XO_TEST_CASE( _name_ ) \
	static void _name_##_function( ::xo::test::test_case& ); \
	auto _name_##_test_case = ::xo::test::test_case( #_name_, &_name_##_function ); \
	static void _name_##_function( ::xo::test::test_case& XO_ACTIVE_TEST_CASE )

#define XO_TEST_CASE_SKIP( _name_ ) \
	static void _name_( ::xo::test::test_case& XO_ACTIVE_TEST_CASE )

#define XO_CHECK( _operation_ ) \
	try { bool _result_ = ( _operation_ ); XO_ACTIVE_TEST_CASE.check( _result_, #_operation_ ); } \
	catch( std::exception& e ) { XO_ACTIVE_TEST_CASE.check( false, #_operation_, e.what() ); }

#define XO_CHECK_MESSAGE( _operation_, _message_ ) \
	try { bool _result_ = ( _operation_ ); XO_ACTIVE_TEST_CASE.check( _result_, #_operation_, _message_ ); } \
	catch( std::exception& e ) { XO_ACTIVE_TEST_CASE.check( false, #_operation_, e.what() ); }

namespace xo
{
	namespace test
	{
		struct test_result {
			int checks_ = 0;
			int passed_ = 0;
			int failed_ = 0;
			string error_;
			const bool success() const { return failed_ == 0 && checks_ > 0 && error_.empty(); }
			test_result& operator+=( const test_result& o ) {
				checks_ += o.checks_;
				passed_ += o.passed_;
				failed_ += o.failed_;
				error_ += error_.empty() ? o.error_ : "; " + o.error_;
				return *this;
			}
		};

		class test_case;
		typedef void( *test_func_t )( test_case& );
		class XO_API test_case
		{
		public:
			test_case( const char* name, test_func_t func );
			bool check( bool result, const char* operation, const string& message = "" );
			const test_result& run();

		private:
			bool try_run_func();
			const char* name_;
			test_func_t func_;
			test_result result_;
		};

		XO_API void register_test_case( test_case* tc );
		XO_API int run_all_test_cases();
	}
}
