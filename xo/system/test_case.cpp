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

		int run_all()
		{
			int tests_failed = 0;
			int total = static_cast<int>( get_test_cases().size() );

			int checks_total = 0;
			int checks_passed = 0;
			int checks_failed = 0;
			for ( auto tc : get_test_cases() )
				tests_failed += tc->run( &checks_total, &checks_passed, &checks_failed );

			auto level = tests_failed > 0 ? log::error_level : log::info_level;
			if ( tests_failed > 0 )
				log::error( "WARNING: ", tests_failed, " of ", total, " tests FAILED!" );
			else
				log::info( "Performed ", total, " tests with ", checks_passed, " checks; ALL CLEAR :-)" );

#if XO_DEBUG_MODE && defined( XO_COMP_MSVC )
			xo::wait_for_key();
#endif

			return tests_failed;
		}

		test_case::test_case( const char* name, test_func_t func ) : func_( func ), name_( name ), num_checks_( 0 ), num_passed_( 0 ), num_failed_( 0 )
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

		int test_case::run( int* checks, int* passed, int* failed )
		{
			num_checks_ = 0;
			num_passed_ = 0;
			num_failed_ = 0;
			log::info( "TEST CASE: ", name_ );
			try
			{
				func_( *this );
				if ( checks ) *checks += num_checks_;
				if ( passed ) *passed += num_passed_;
				if ( failed ) *failed += num_failed_;

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
