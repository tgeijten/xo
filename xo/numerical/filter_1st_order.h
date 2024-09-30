#pragma once

#include <cmath>

namespace xo
{
	template< typename T, typename TimeT = float >
	class filter_1st_order {
	public:
		filter_1st_order( TimeT rate, T init_value = T() ) : value_( init_value ), rate_( rate ) {};

		T operator()( const T& value, TimeT dt ) { return update_accurate( value, dt ); }

		T update_accurate( const T& value, TimeT dt ) {
			return value_ = value + ( value_ - value ) * std::exp( -rate_ * dt );
		}

		T update_fast( const T& value, TimeT dt ) {
			return value_ = value_ + dt * rate_ * ( value - value_ );
		}

		const T& value() const { return value_; }
		const TimeT& rate() const { return rate_; }

		void set_value( const T& value ) { value_ = value; }
		void set_rate( const TimeT& rate ) { rate_ = rate; }

	private:
		T value_;
		TimeT rate_;
	};
}
