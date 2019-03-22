#pragma once

#include <array>

namespace xo
{
	template< typename T, size_t D >
	struct polynomial
	{
		polynomial() : coeff() { static_assert( D > 0, "polynomial degree must be > 0" ); }
		polynomial( const polynomial& o ) : coeff( o.coeff ) {}
		template< typename... E > explicit polynomial( E... e ) : coeff{ { e... } } { static_assert( D > 0, "polynomial degree must be > 0" ); }

		polynomial& operator=( const polynomial& o ) { coeff = o.coeff; return *this; }

		T operator()( const T& v ) const {
			T result = coeff[ 0 ];
			T factor = T( v );
			for ( auto it = std::begin( coeff ) + 1; it != std::end( coeff ); ++it ) { result += factor * *it; factor *= v; }
			return result;
		}

		T& operator[]( size_t idx ) { return coeff[ idx ]; }
		const T& operator[]( size_t idx ) const { return coeff[ idx ]; }

		const T& offset() const { return coeff[ 0 ]; }
		const T& slope() const { return coeff[ 1 ]; }
		T& offset() { return coeff[ 0 ]; }
		T& slope() { return coeff[ 1 ]; }

	private:
		std::array< T, D + 1 > coeff;
	};

	template< typename T > using linear_function = polynomial< T, 1 >;
	template< typename T > using quadratic_function = polynomial< T, 2 >;
	template< typename T > using cubic_function = polynomial< T, 3 >;

	template< typename T > T intersect_y( const linear_function< T >& f, T y ) { return ( y - f.offset() ) / f.slope(); }
}
