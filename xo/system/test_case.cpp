#include "test_case.h"
#include "xo/system/log.h"
#include "xo/system/assert.h"
#include "xo/container/container_tools.h"
#include "system_tools.h"

namespace xo
{
	namespace test
	{
		vector< test_case* >& get_test_cases()
		{
			static vector< test_case* > test_cases;
			return test_cases;
		}

		void register_test_case( test_case* tc )
		{
			xo_error_if( find( get_test_cases(), tc ) != get_test_cases().end(), "Test case already registered" );
			get_test_cases().emplace_back( tc );
		}

		test_case::test_case( const char* name, test_func_t func ) :
			name_( name ) ,
			func_( func )
		{
			register_test_case( this );
		}

		bool test_case::check( bool result, const char* operation, const string& message )
		{
			result_.checks_++;
			if ( result )
			{
				result_.passed_++;
				return true;
			} else {
				result_.failed_++;
				log::error( name_, ": CHECK FAILED: ", operation, " ", message );
				return false;
			}
		}

		bool test_case::try_run_func()
		{
			try
			{
				func_( *this );
				return true;
			}
			catch ( std::exception& e )
			{
				result_.error_ = e.what();
				return false;
			}
		}

		const test_result& test_case::run()
		{
			if ( try_run_func() )
			{
				if ( result_.success() )
					log::info( name_, ": ", result_.passed_, " checks passed" );
				else
					log::error( name_, ": ", result_.failed_, " of ", result_.checks_, " checks FAILED!" );
			}
			else log::critical( name_, ": EXCEPTION: ", result_.error_ );

			return result_;
		}

		int run_all_test_cases()
		{
			test_result total;
			int tests_failed = 0;
			int tests_performed = 0;
			for ( auto tc : get_test_cases() )
			{
				auto& r = tc->run();
				tests_performed += 1;
				tests_failed += int( !r.success() );
				total += r;
			}

			if ( tests_failed == 0 )
				log::info( "Performed ", tests_performed, " tests with ", total.passed_, " checks; ALL CLEAR :-)" );
			else
				log::error( "WARNING: ", tests_failed, " of ", tests_performed, " tests FAILED!" );

			return tests_failed;
		}
	}
}
