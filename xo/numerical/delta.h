#pragma once

namespace xo
{
	template< typename T >
	struct delta
	{
		delta() : previous_( 0 ) {}
		delta( const T& initial_value ) : previous_( initial_value ) {}
		T operator()( const T& new_value ) {
			T result = new_value - previous_;
			previous_ = new_value;
			return result;
		}

	private:
		T previous_;
	};
}
