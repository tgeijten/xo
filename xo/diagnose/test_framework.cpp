#include "test_framework.h"
#include "xo/system/log.h"
#include "xo/system/assert.h"
#include "xo/container/container_tools.h"

namespace xo
{
	namespace test
	{
		std::vector< test_case* >& get_test_cases()
		{
			static std::vector< test_case* > test_cases;
			return test_cases;
		}

		void register_test_case( test_case* tc )
		{
			xo_error_if( find( get_test_cases(), tc ) != get_test_cases().end(), "Test case already registered" );
			get_test_cases().emplace_back( tc );
		}

		int run_all()
		{
			int failed = 0;
			int total = static_cast<int>( get_test_cases().size() );
			for ( auto tc : get_test_cases() )
				failed += tc->run();

			auto level = failed > 0 ? log::error_level : log::info_level;
			log::message( level, "Test results: ", total, " performed, ", failed, " failed, ", total - failed, " passed" );

			return failed;
		}

		test::test_case::test_case( const char* name, test_func_t func ) : func_( func ), name_( name ), num_checks_( 0 ), num_passed_( 0 ), num_failed_( 0 )
		{
			register_test_case( this );
		}

		bool test_case::check( bool result, const char* operation, const string& message )
		{
			++num_checks_;
			if ( result )
			{
				num_passed_++;
				return true;
			}
			else
			{
				log::error( "\t", "FAILED: ", operation, " ", message );
				num_failed_++;
				return false;
			}
		}

		int test_case::run()
		{
			num_checks_ = 0;
			num_passed_ = 0;
			num_failed_ = 0;
			log::info( "TEST CASE: ", name_ );
			try
			{
				func_( *this );
				return num_failed_ == 0 ? 0 : 1;
			}
			catch ( std::exception& e )
			{
				log::error( "\t", "EXCEPTION: ", e.what() );
				return 1;
			}
		}
	}
}
