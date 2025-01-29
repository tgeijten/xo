#pragma once

namespace xo
{
	/// base class for arithmetic value types, such as angle
	/// T: primitive type, C: type of derived class
	template< typename T, typename C >
	struct arithmetic {
		constexpr arithmetic() : value() {}
		constexpr arithmetic( no_init_t ) {}
		explicit constexpr arithmetic( const T& v ) : value( v ) {}

		/// scalar multiplication / division
		template< typename S > C& operator*=( const S& s ) { value *= T( s ); return derived(); }
		template< typename S > C& operator/=( const S& s ) { value /= T( s ); return derived(); }
		C operator*( const arithmetic& o ) const { return C( value * o.value ); }
		C operator*( const T& s ) const { return C( value * s ); }
		C operator/( const T& s ) const { return C( value / s ); }

		/// addition / subtraction
		C& operator+=( const arithmetic& o ) { value += o.value; return derived(); }
		C& operator-=( const arithmetic& o ) { value -= o.value; return derived(); }
		constexpr C operator+( const arithmetic& o ) const { return C( value + o.value ); }
		constexpr C operator-( const arithmetic& o ) const { return C( value - o.value ); }

		/// negation
		constexpr C operator-() const { return C( -value ); }

		/// comparison operators
		constexpr bool operator>( const arithmetic& o ) const { return value > o.value; }
		constexpr bool operator<( const arithmetic& o ) const { return value < o.value; }
		constexpr bool operator>=( const arithmetic& o ) const { return value >= o.value; }
		constexpr bool operator<=( const arithmetic& o ) const { return value <= o.value; }
		constexpr bool operator==( const arithmetic& o ) const { return value == o.value; }
		constexpr bool operator!=( const arithmetic& o ) const { return value != o.value; }

		/// cast to derived class type
		C& derived() { return static_cast<C&>( *this ); }
		const C& derived() const { return static_cast<const C&>( *this ); }

		/// value
		T value;
	};

	/// scalar multiplication
	template< typename T, typename C >
	C operator*( const T& s, const arithmetic<T, C>& a ) { return C( s * a.value ); }

	/// scalar division
	template< typename T, typename C >
	C operator/( const T& s, const arithmetic<T, C>& a ) { return C( s / a.value ); }

	/// absolute value
	template< typename T, typename C >
	C abs( const arithmetic<T, C>& v ) { return C( v.value < T( 0 ) ? -v.value : v.value ); }
}
