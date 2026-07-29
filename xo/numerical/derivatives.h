#pragma once

namespace xo
{
	template <typename T, typename TimeT = double>
	struct derivatives
	{
		void update( T value, TimeT time )
		{
			const TimeT dt = time - time_;
			if ( dt > TimeT( 0 ) ) {
				++samples_;
				if ( samples_ > 1 ) {
					const T velocity = ( value - value_ ) / dt;
					if ( samples_ > 2 )
						acceleration_ = 2 * ( velocity - velocity_ ) / ( dt + dt_ );
					velocity_ = velocity;
				}
				time_ = time;
				dt_ = dt;
			}
			value_ = value;
		}

		const T& value() const { return value_; }
		const T& velocity() const { return velocity_; }
		const T& acceleration() const { return acceleration_; }
		const TimeT& timestamp() const { return time_; }

	private:
		T value_{};
		T velocity_{};
		T acceleration_{};
		TimeT time_{};
		TimeT dt_{};
		size_t samples_{};
	};
}

