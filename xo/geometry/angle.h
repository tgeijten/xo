#pragma once

#include "xo/xo_types.h"
#include "xo/geometry/angle_type.h"
#include "xo/numerical/constants.h"
#include <cmath>

namespace xo
{
	template< typename T > T rad_to_deg( T rad_value ) { return ( T( 180 ) / constants<T>::pi() ) * rad_value; }
	template< typename T > T deg_to_rad( T deg_value ) { return ( constants<T>::pi() / T( 180 ) ) * deg_value; }

	template< angle_unit A, angle_unit B > struct angle_converter
	{ template< typename T > static constexpr T convert( T v ) { return v; } };

	template<> struct angle_converter< angle_unit::degrees, angle_unit::radians >
	{ template< typename T > static constexpr T convert( T v ) { return deg_to_rad( v ); } };

	template<> struct angle_converter< angle_unit::radians, angle_unit::degrees >
	{ template< typename T > static constexpr T convert( T v ) { return rad_to_deg( v ); } };

	template< angle_unit U, typename T >
	struct angle_
	{
		/// default constructor
		explicit constexpr angle_() : value( T() ) {}

		/// constructor taking any value type
		template< typename T2 > explicit constexpr angle_( const T2& v ) : value( T( v ) ) {}

		/// constructor that takes angle, converts unit and value type
		template< typename T2 > constexpr angle_( const angle_<angle_unit::degrees, T2>& a ) : value( angle_converter<angle_unit::degrees, U>::convert( T( a.value ) ) ) {}
		template< typename T2 > constexpr angle_( const angle_<angle_unit::radians, T2>& a ) : value( angle_converter<angle_unit::radians, U>::convert( T( a.value ) ) ) {}

		/// assignment that takes angle, converts unit and value type
		template< typename T2 > angle_<U, T>& operator=( const angle_<angle_unit::degrees, T2>& a ) { value = angle_converter<angle_unit::degrees, U>::convert( T( a.value ) ); return *this; }
		template< typename T2 > angle_<U, T>& operator=( const angle_<angle_unit::radians, T2>& a ) { value = angle_converter<angle_unit::radians, U>::convert( T( a.value ) ); return *this; }

		/// get rad / deg value
		T deg_value() const { return angle_converter< U, angle_unit::degrees >::convert( value ); }
		T rad_value() const { return angle_converter< U, angle_unit::radians >::convert( value ); }

		/// scalar multiplication and division
		template< typename TS > angle_& operator*=( const TS& s ) { value *= T(s); return *this; }
		angle_ operator*( const angle_& s ) const { return angle_( value * s.value ); }
		template< typename TS > angle_ operator*( const TS& s ) const { return angle_( value * T(s) ); }
		template< typename TS > angle_& operator/=( const TS& s ) { value /= T(s); return *this; }
		template< typename TS > angle_ operator/( const TS& s ) const { return angle_( value / T(s) ); }

		/// addition / subtraction of an angle with the same units
		angle_& operator+=( const angle_& other ) { value += other.value; return *this; }
		angle_& operator-=( const angle_& other ) { value -= other.value; return *this; }

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

	// alias names
	template < typename T > using radian_ = angle_<angle_unit::radians, T>;
	template < typename T > using degree_ = angle_<angle_unit::degrees, T>;
	using radianf = radian_<float>;
	using radiand = radian_<double>;
	using degreef = degree_<float>;
	using degreed = degree_<double>;

	template< typename T > angle_<angle_unit::radians, T> rad( T rad ) { return angle_<angle_unit::radians, T>( rad ); }
	template< typename T > angle_<angle_unit::degrees, T> deg( T deg ) { return angle_<angle_unit::degrees, T>( deg ); }

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

	/// scalar multiplication
	template< angle_unit U, typename T, typename TS >
	angle_<U, T> operator*( const TS& s, const angle_<U, T>& a ) { return angle_<U, T>( T(s) * a.value ); }

	/// scalar division
	template< angle_unit U, typename T, typename TS >
	angle_<U, T> operator/( const TS& s, const angle_<U, T>& a ) { return angle_<U, T>( T(s) / a.value ); }

	/// addition, value types can be different
	template< angle_unit U, typename T1, typename T2 >
	auto operator+( const angle_<U, T1>& a1, const angle_<U, T2>& a2 ) -> angle_<U, decltype( a1.value + a2.value )>
	{ return angle_<U, decltype( a1.value + a2.value )>( a1.value + a2.value ); }

	/// subtraction, value types can be different
	template< angle_unit U, typename T1, typename T2 >
	auto operator-( const angle_<U, T1>& a1, const angle_<U, T2>& a2 ) -> angle_<U, decltype( a1.value - a2.value )>
	{ return angle_<U, decltype( a1.value - a2.value )>( a1.value - a2.value ); }

	/// multiplication
	template< angle_unit U, typename T >
	angle_<U, T> operator*( const angle_<U, T>& a, const angle_<U, T>& b ) { return angle_<U, T>( a.value * b.value ); }

	/// absolute value
	template< angle_unit U, typename T >
	angle_<U, T> abs( const angle_<U, T>& v ) { return angle_<U, T>( std::abs( v.value ) ); }

	/// sin/cos/tan
	template< angle_unit U, typename T > T sin( const angle_<U, T>& a ) { return std::sin( a.rad_value() ); }
	template< angle_unit U, typename T > T cos( const angle_<U, T>& a ) { return std::cos( a.rad_value() ); }
	template< angle_unit U, typename T > T tan( const angle_<U, T>& a ) { return std::tan( a.rad_value() ); }
	template< angle_unit U, typename T > T asin( const angle_<U, T>& a ) { return std::asin( a.rad_value() ); }
	template< angle_unit U, typename T > T acos( const angle_<U, T>& a ) { return std::acos( a.rad_value() ); }
	template< angle_unit U, typename T > T atan( const angle_<U, T>& a ) { return std::atan( a.rad_value() ); }

	/// Make vec3 from polar angles (radius, inclination & azimuth)
	template< angle_unit U, typename T > vec3_<T> vec3_from_polar( T radius, angle_<U, T> inc, angle_<U, T> azi )
	{ return vec3_<T>( radius * sin( inc ) * sin( azi ), radius * cos( inc ), radius * sin( inc ) * cos( azi ) ); }
}
