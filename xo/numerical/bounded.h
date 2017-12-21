#pragma once

namespace xo
{
	template < typename T, T L, T U >
	struct bounded
	{
		bounded() : value_( MIN ) {}
		bounded( T value ) { set( value ); }
		bounded< T, L, U >& operator=( T value ) { set( value ); }
		bounded< T, L, U >& operator=( bounded< T, L, U >& ) { set( value ); }
		bounded< T, L, U >& set( T value ) { if ( value < L || value > U ) flut_error( "Value out of bounds" ); else value_ = value; }

		operator const T&() const { return value_; }
		operator T&() { return value_; }

		T value_;
	};
}
