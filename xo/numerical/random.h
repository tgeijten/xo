#pragma once

#include "xo/system/prerequisites.h"
#include <random>

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
		template< typename T > T uni( T min = T( 0 ), T max = T( 1 ) ) { return std::uniform_real_distribution<T>( min, max )( engine ); }
		template< typename T > T uni_int( T min = T( 0 ), T max = T( 1 ) ) { return std::uniform_int_distribution<T>( min, max )( engine ); }
		template< typename T > T norm( T mean, T stdev ) { return std::normal_distribution<T>( mean, stdev )( engine ); }

		static random_number_generator& global_instance() { return global_generator; }

	private:
		// #TODO: use faster & less accurate engine
		std::default_random_engine engine;
		static random_number_generator global_generator;
	};

	template< typename T > T rand_uni( T min, T max ) { return random_number_generator::global_instance().uni( min, max ); }
	template< typename T > T rand_uni_int( T min, T max ) { return random_number_generator::global_instance().uni_int( min, max ); }
	template< typename T > T rand_norm( T mean, T stdev ) { return random_number_generator::global_instance().norm( mean, stdev ); }
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
