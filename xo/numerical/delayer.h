#pragma once

#include <array>
#include "xo/xo_types.h"
#include "xo/system/assert.h"

namespace xo
{
	template< typename T, int N = 2 >
	struct delayer
	{
		delayer( float delay = 1.0f, const T& init_value = T() ) :
			window_( delay / ( N - 1 ) ),
			time_( 0 ),
			idx_( 0 ),
			data_{ init_value }
		{ static_assert( N >= 2 ); xo_assert( delay > 0 ); }

		void add_sample( const T& value, float delta_time ) {
			time_ += delta_time;
			if ( time_ + 0.5f * delta_time > window_ ) {
				time_ -= window_;
				++idx_ %= N;
				data_[idx_] = value;
			}
		}

		T delayed_value() const {
			auto w = time_ / window_;
			return w * data_[( idx_ + N + 2 ) % N] + ( 1.0f - w ) * data_[( idx_ + N + 1 ) % N];
		}

		T operator()( const T& value, float delta_time ) { add_sample( value, delta_time ); return delayed_value(); }
		T operator()() { return delayed_value(); }

		float window_;
		float time_;
		index_t idx_;
		std::array< T, N > data_;
	};

	template< typename T, int N = 2 >
	struct smooth_delayer
	{
		smooth_delayer( float delay = 1.0f, const T& init_value = T() ) :
			window_( delay / ( N - 0.5f ) ),
			time_( 0 ),
			inter_( 0 ),
			idx_( 0 ),
			data_{ init_value }
		{ static_assert( N >= 2 ); xo_assert( delay > 0 ); }

		void add_sample( const T& value, float delta_time ) {
			time_ += delta_time;
			if ( time_ >= window_ ) {
				inter_ += ( window_ - time_ + delta_time ) * value;
				++idx_ %= N;
				data_[idx_] = inter_ / window_;
				time_ -= window_;
				inter_ = time_ * value;
			}
			else inter_ += value * delta_time;
		}

		T delayed_value() const {
			auto w = time_ / window_;
			return w * data_[( idx_ + N + 2 ) % N] + ( 1.0f - w ) * data_[( idx_ + N + 1 ) % N];
		}

		T operator()( const T& value, float delta_time ) { add_sample( value, delta_time ); return delayed_value(); }
		T operator()() { return delayed_value(); }

		float window_;
		float time_;
		T inter_;
		index_t idx_;
		std::array< T, N > data_;
	};
}
