#pragma once

namespace xo
{
	enum class angle_unit { degrees, radians };
	template< angle_unit U, typename T > struct angle_;

	template< typename T > T rad_to_deg( T rad_value ) { return T( 57.295779513082320877L ) * rad_value; }
	template< typename T > T deg_to_rad( T deg_value ) { return T( 0.017453292519943295769L ) * deg_value; }

	template< angle_unit A, angle_unit B > struct angle_converter
	{
		template< typename T > static constexpr T convert( T v ) { return v; }
	};

	template<> struct angle_converter< angle_unit::degrees, angle_unit::radians >
	{
		template< typename T > static constexpr T convert( T v ) { return deg_to_rad( v ); }
	};

	template<> struct angle_converter< angle_unit::radians, angle_unit::degrees >
	{
		template< typename T > static constexpr T convert( T v ) { return rad_to_deg( v ); }
	};

	template< angle_unit U, typename T >
	struct angle_
	{
		/// default constructor
		constexpr angle_() : value( T() ) {}

		/// conversion / copy constructors with same T
		constexpr angle_( const angle_<angle_unit::degrees, T>& a ) : value( angle_converter<angle_unit::degrees, U>::convert( T( a.value ) ) ) {}
		constexpr angle_( const angle_<angle_unit::radians, T>& a ) : value( angle_converter<angle_unit::radians, U>::convert( T( a.value ) ) ) {}

		/// explicit conversion / copy constructors with different T
		template< typename T2 > explicit constexpr angle_( const T2& v ) : value( T( v ) ) {}
		template< typename T2 > explicit constexpr angle_( const angle_<angle_unit::degrees, T2>& a ) : value( angle_converter<angle_unit::degrees, U>::convert( T( a.value ) ) ) {}
		template< typename T2 > explicit constexpr angle_( const angle_<angle_unit::radians, T2>& a ) : value( angle_converter<angle_unit::radians, U>::convert( T( a.value ) ) ) {}

		/// copy assignment
		angle_& operator=( const angle_& a ) { value = a.value; return *this; }

		/// get rad / deg value
		T deg_value() const { return angle_converter< U, angle_unit::degrees >::convert( value ); }
		T rad_value() const { return angle_converter< U, angle_unit::radians >::convert( value ); }

		/// scalar multiplication and division
		angle_& operator*=( const T& s ) { value *= T( s ); return *this; }
		angle_& operator/=( const T& s ) { value /= T( s ); return *this; }
		angle_ operator*( const angle_& s ) const { return angle_( value * s.value ); }
		angle_ operator*( const T& s ) const { return angle_( value * s ); }
		angle_ operator/( const T& s ) const { return angle_( value / s ); }

		/// addition / subtraction of an angle with the same units
		angle_& operator+=( const angle_& other ) { value += other.value; return *this; }
		angle_& operator-=( const angle_& other ) { value -= other.value; return *this; }
		angle_ operator+( const angle_& other ) const { return angle_( value + other.value ); }
		angle_ operator-( const angle_& other ) const { return angle_( value - other.value ); }

		/// negation
		constexpr angle_ operator-() const { return angle_( -value ); }

		/// comparison operators
		bool operator>( const angle_& other ) const { return value > other.value; }
		bool operator<( const angle_& other ) const { return value < other.value; }
		bool operator>=( const angle_& other ) const { return value >= other.value; }
		bool operator<=( const angle_& other ) const { return value <= other.value; }
		bool operator==( const angle_& other ) const { return value == other.value; }
		bool operator!=( const angle_& other ) const { return value != other.value; }

		/// actual value
		T value;
	};

	/// scalar multiplication
	template< angle_unit U, typename T >
	angle_<U, T> operator*( const T& s, const angle_<U, T>& a ) { return angle_<U, T>( s * a.value ); }

	/// scalar division
	template< angle_unit U, typename T >
	angle_<U, T> operator/( const T& s, const angle_<U, T>& a ) { return angle_<U, T>( s / a.value ); }

	// alias names
	template < typename T > using radian_ = angle_<angle_unit::radians, T>;
	template < typename T > using degree_ = angle_<angle_unit::degrees, T>;
	using radianf = radian_<float>;
	using radiand = radian_<double>;
	using degreef = degree_<float>;
	using degreed = degree_<double>;

	inline namespace literals {
		/// user-defined literals, add 'using namespace xo::literals' to access them outside the xo namespace
		inline constexpr degreef operator"" _degf( long double v ) { return degreef( v ); }
		inline constexpr degreef operator"" _degf( unsigned long long int v ) { return degreef( v ); }
		inline constexpr radianf operator"" _radf( long double v ) { return radianf( v ); }
		inline constexpr radianf operator"" _radf( unsigned long long int v ) { return radianf( v ); }
		inline constexpr degreed operator"" _deg( long double v ) { return degreed( v ); }
		inline constexpr degreed operator"" _deg( unsigned long long int v ) { return degreed( v ); }
		inline constexpr radiand operator"" _rad( long double v ) { return radiand( v ); }
		inline constexpr radiand operator"" _rad( unsigned long long int v ) { return radiand( v ); }
	}
}
