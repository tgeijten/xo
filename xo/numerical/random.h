#pragma once

#include "xo/system/xo_config.h"
#include "bounds.h"
#include "xo/geometry/angle.h"

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
		template< angle_unit U, typename T > angle_<U, T> uniform( angle_<U, T> min, angle_<U, T> max ) { return angle_<U, T>( uniform( min.value, max.value ) ); }
		template< typename T > T uniform( const bounds<T>& b ) { return uniform( b.lower, b.upper ); }

		template< typename T > T uniform_int( T min, T max ) { return std::uniform_int_distribution<T>( min, max )( re ); }
		template< typename T > T uniform_int( const bounds<T>& b ) { return uniform_int( b.lower, b.upper ); }

		template< typename T > T normal( T mean, T stdev ) { return std::normal_distribution<T>( mean, stdev )( re ); }

		template< typename T > vec3_<T> uniform_vec3( T min, T max ) {
			std::uniform_real_distribution<T> rd{ min, max }; return { rd( re ), rd( re ), rd( re ) };
		}
		template< typename T > vec3_<T> uniform_vec3( const bounds<T>& b ) {
			return uniform_vec3( b.lower, b.upper );
		}
		template< typename T > vec3_<T> uniform_vec3( const vec3_<bounds<T>>& b ) {
			return { uniform( b.x ), uniform( b.y ), uniform( b.z ) };
		}

		template< typename T > quat_<T> uniform_quat( degree_<T> min, degree_<T> max ) {
			return quat_from_euler_xyz( vec3deg_<T>( uniform_vec3( min.value, max.value ) ) );
		}
		template< typename T > quat_<T> uniform_quat( const bounds<degree_<T>>& b ) {
			return uniform_quat( b.lower, b.upper );
		}
		template< typename T > quat_<T> uniform_quat( const vec3_<bounds<degree_<T>>>& b ) {
			return quat_from_euler_xyz( uniform_vec3( b ) );
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
