#pragma once

#include "xo/system/xo_config.h"
#include "bounds.h"

#include <random>

namespace xo
{
	// #todo: std::default_random_engine is mt19937 on msvc and minstd_rand0 on gcc
	// #todo: normal distribution is different on msvc and gcc (order is reversed)
	template< typename RandomEngineT >
	struct random_number_generator_
	{
		random_number_generator_( unsigned int seed = 123 ) : re( seed ) {}
		void seed( unsigned int s ) { re.seed( s ); }

		template< typename T > T uniform( T min, T max ) { return std::uniform_real_distribution<T>( min, max )( re ); }
		template< typename T > T uniform( const xo::bounds<T>& b ) { return uniform<T>( b.lower, b.upper ); }

		template< typename T > T uniform_int( T min, T max ) { return std::uniform_int_distribution<T>( min, max )( re ); }
		template< typename T > T uniform_int( const xo::bounds<T>& b ) { return uniform_int<T>( b.lower, b.upper ); }

		template< typename T > T normal( T mean, T stdev ) { return std::normal_distribution<T>( mean, stdev )( re ); }

		template< typename T > vec3_<T> uniform_vec3( T min, T max ) {
			std::uniform_real_distribution<T> rd{ min, max }; return { rd( re ), rd( re ), rd( re ) };
		}
		template< typename T > quat_<T> uniform_quat( degree_<T> min, degree_<T> max ) {
			auto ea = vec3deg_<T>( uniform_vec3<T>( min.value, max.value ) ); return quat_from_euler_xyz( ea );
		}

	private:
		RandomEngineT re;
	};

	using random_number_generator_default = random_number_generator_<std::default_random_engine>;
	using random_number_generator_fast = random_number_generator_<std::minstd_rand>;
	using random_number_generator_accurate = random_number_generator_<std::mt19937>;
	using random_number_generator_default_msvc = random_number_generator_<std::mt19937>;
	using random_number_generator_default_gcc = random_number_generator_<std::minstd_rand0>;
}
