#pragma once

#include "xo/system/xo_config.h"
#include "xo/utility/sfinae_tools.h"
#include "bounds.h"

#include <random> // #TODO: try to get rid of this header

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	struct XO_API random_number_generator
	{
		random_number_generator( unsigned int seed = 123 ) : engine( seed ) {}
		void seed( unsigned int s ) { engine.seed( s ); }

		template< typename T, XO_ENABLE_IF_FLOATING_POINT >	T uni( T min, T max ) { return std::uniform_real_distribution<T>( min, max )( engine ); }
		template< typename T, XO_ENABLE_IF_FLOATING_POINT >	T uni( const xo::bounds< T >& b ) { return std::uniform_real_distribution<T>( b.lower, b.upper )( engine ); }
		template< typename T, XO_ENABLE_IF_INTEGRAL > T uni( T min, T max ) { return std::uniform_int_distribution<T>( min, max )( engine ); }
		template< typename T, XO_ENABLE_IF_INTEGRAL > T uni( const xo::bounds< T >& b ) { return std::uniform_int_distribution<T>( b.lower, b.upper )( engine ); }
		template< typename T, XO_ENABLE_IF_FLOATING_POINT  > T norm( T mean, T stdev ) { return std::normal_distribution<T>( mean, stdev )( engine ); }

	private:
		std::default_random_engine engine;
	};

	XO_API random_number_generator& global_random_number_generator();
	template< typename T, XO_ENABLE_IF_FLOATING_POINT > T rand_uni( T min, T max ) { return global_random_number_generator().uni( min, max ); }
	template< typename T, XO_ENABLE_IF_INTEGRAL > T rand_uni_int( T min, T max ) { return global_random_number_generator().uni( min, max ); }
	template< typename T, XO_ENABLE_IF_FLOATING_POINT > T rand_norm( T mean, T stdev ) { return global_random_number_generator().norm( mean, stdev ); }
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
