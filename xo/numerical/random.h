#pragma once

#include "xo/system/xo_config.h"
#include "xo/utility/sfinae_tools.h"
#include "bounds.h"

#include <random> // #todo: try to get rid of this header
#include "xo/geometry/angle_type.h"

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
	template< typename T, XO_ENABLE_IF_FLOATING_POINT > T rand_uni( const xo::bounds<T>& b ) { return global_random_number_generator().uni( b.lower, b.upper ); }
	template< typename T, XO_ENABLE_IF_INTEGRAL > T rand_uni_int( T min, T max ) { return global_random_number_generator().uni( min, max ); }
	template< typename T, XO_ENABLE_IF_INTEGRAL > T rand_uni_int( const xo::bounds<T>& b ) { return global_random_number_generator().uni( b.lower, b.upper ); }
	template< typename T, XO_ENABLE_IF_FLOATING_POINT > T rand_norm( T mean, T stdev ) { return global_random_number_generator().norm( mean, stdev ); }

	template< typename T > vec3_<T> rand_vec3( T min = T( -1 ), T max = T( 1 ), random_number_generator& rng = global_random_number_generator() ) {
		return vec3_<T>{ rng.uni( min, max ), rng.uni( min, max ), rng.uni( min, max ) };
	}
}
