#pragma once

namespace xo
{
	/// base class for arithmetic value types, such as degree and radian
	/// T: primitive type, C: type of derived class
	template< typename T, typename C >
	struct arithmetic {
		constexpr arithmetic() : value() {}
		explicit constexpr arithmetic( const T& v ) : value( v ) {}

		/// scalar multiplication / division
		template< typename S > C& operator*=( const S& s ) { value *= T( s ); return derived(); }
		template< typename S > C& operator/=( const S& s ) { value /= T( s ); return derived(); }
		C operator*( const arithmetic& other ) { return C( value * other.value ); }

		/// addition / subtraction
		C operator+( const arithmetic& other ) { return C( value + other.value ); }
		C operator-( const arithmetic& other ) { return C( value - other.value ); }
		C& operator+=( const arithmetic& other ) { value += other.value; return derived(); }
		C& operator-=( const arithmetic& other ) { value -= other.value; return derived(); }

		/// negation
		constexpr C operator-() const { return C( -value ); }

		/// comparison operators
		bool operator>( const arithmetic& other ) const { return value > other.value; }
		bool operator<( const arithmetic& other ) const { return value < other.value; }
		bool operator>=( const arithmetic& other ) const { return value >= other.value; }
		bool operator<=( const arithmetic& other ) const { return value <= other.value; }
		bool operator==( const arithmetic& other ) const { return value == other.value; }
		bool operator!=( const arithmetic& other ) const { return value != other.value; }

		C& derived() { return static_cast<C&>( *this ); }
		const C& derived() const { return static_cast<const C&>( *this ); }

		T value;
	};

	/// scalar multiplication
	template< typename T, typename C >
	C operator*( const T& s, const arithmetic<T, C>& a ) { return C( s * a.value ); }
	template< typename T, typename C >
	C operator*( const arithmetic<T, C>& a, const T& s ) { return C( a.value * s ); }

	/// scalar division
	template< typename T, typename C >
	C operator/( const T& s, const arithmetic<T, C>& a ) { return C( s / a.value ); }
	template< typename T, typename C >
	C operator/( const arithmetic<T, C>& a, const T& s ) { return C( a.value / s ); }

	/// addition
	template< typename T, typename C >
	C operator+( const arithmetic<T, C>& a1, const arithmetic<T, C>& a2 ) { return C( a1.value + a2.value ); }

	/// subtraction
	template< typename T, typename C >
	C operator-( const arithmetic<T, C>& a1, const arithmetic<T, C>& a2 ) { return C( a1.value - a2.value ); }

	/// absolute value
	template< typename T, typename C >
	C abs( const arithmetic<T, C>& v ) { return C( v.value < 0 ? -v.value : v.value ); }
}
