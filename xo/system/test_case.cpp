#include "test_case.h"
#include "xo/system/log.h"
#include "xo/system/assert.h"
#include "xo/container/container_tools.h"
#include "system_tools.h"
#include "xo/utility/pointer_types.h"
#include <future>
#include "xo/time/timer.h"
#include "xo/string/string_tools.h"

namespace xo
{
	namespace test
	{
		std::mutex g_log_mutex;

		vector< u_ptr< test_case > >& get_test_cases()
		{
			static vector< u_ptr< test_case > > test_cases;
			return test_cases;
		}

		index_t add_test_case( const string& name, test_func_t fn )
		{
			get_test_cases().emplace_back( new test_case( name, fn ) );
			return get_test_cases().size();
		}

		test_case::test_case( const string& name, test_func_t func ) :
			name_( name ) ,
			func_( func )
		{}

		bool test_case::check( bool result, const char* operation, const string& message )
		{
			result_.checks_++;
			if ( result )
			{
				result_.passed_++;
				return true;
			} else {
				auto lock = std::scoped_lock( g_log_mutex );
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
				auto lock = std::scoped_lock( g_log_mutex );
				if ( result_.success() )
					log::info( name_, ": ", result_.passed_, " checks passed" );
				else
					log::error( name_, ": ", result_.failed_, " of ", result_.checks_, " checks FAILED!" );
			}
			else
			{
				auto lock = std::scoped_lock( g_log_mutex );
				log::critical( name_, ": EXCEPTION: ", result_.error_ );
			}

			return result_;
		}

		int run_tests()
		{
			test_result total;
			int tests_failed = 0;
			for ( auto& tc : get_test_cases() )
			{
				const auto& r = tc->run();
				tests_failed += int( !r.success() );
				total += r;
			}

			if ( tests_failed == 0 )
				log::info( "Performed ", get_test_cases().size(), " tests with ", total.passed_, " checks; ALL CLEAR :-)" );
			else
				log::error( "WARNING: ", tests_failed, " of ", get_test_cases().size(), " tests FAILED!" );

			return tests_failed;
		}

		int run_tests_async()
		{
			xo::timer t;

			test_result total;
			std::vector< std::future< const test_result& > > results;
			for ( index_t i = 0; i < get_test_cases().size(); ++i )
				results.push_back( std::async( &test_case::run, get_test_cases()[ i ].get() ) );

			int tests_failed = 0;
			for ( auto& fut : results )
			{
				const auto& r = fut.get();
				tests_failed += int( !r.success() );
				total += r;
			}

			auto time_str = stringf( "%.2fs", t().seconds() );
			if ( tests_failed == 0 )
				log::info( "Performed ", results.size(), " tests with ", total.passed_, " checks in ", time_str, "; ALL CLEAR :-)" );
			else
				log::error( "WARNING: ", tests_failed, " of ", results.size(), " tests FAILED!" );

			return tests_failed;
		}
	}
}
