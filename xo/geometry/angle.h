#pragma once

#include "xo/xo_types.h"
#include "xo/numerical/constants.h"
#include "xo/string/string_cast.h"
#include <cmath>

namespace xo
{
	enum class angle_unit { degrees, radians };

	template< typename T > T rad_to_deg( T rad_value ) { return ( T( 180 ) / constants<T>::pi() ) * rad_value; }
	template< typename T > T deg_to_rad( T deg_value ) { return ( constants<T>::pi() / T( 180 ) ) * deg_value; }

	template< angle_unit A, angle_unit B > struct angle_converter
	{ template< typename T > static const T& convert( const T& v ) { return v; } };

	template<> struct angle_converter< angle_unit::degrees, angle_unit::radians >
	{ template< typename T > static T convert( T v ) { return deg_to_rad( v ); } };

	template<> struct angle_converter< angle_unit::radians, angle_unit::degrees >
	{ template< typename T > static T convert( T v ) { return rad_to_deg( v ); } };

	template< angle_unit U, typename T >
	struct angle_
	{
		/// default constructor
		explicit angle_() : value( T() ) {}

		/// constructor taking any value type
		template< typename T2 > explicit angle_( const T2& v ) : value( T( v ) ) {}

		/// copy constructor
		explicit angle_( const angle_<U, T>& a ) : value( a.value ) {}

		/// constructor that takes angle, converts unit and value type
		template< angle_unit U2, typename T2 > angle_( const angle_<U2, T2>& a ) : value( angle_converter<U2, U>::convert( T( a.value ) ) ) {}

		/// assignment that takes angle, converts unit and value type
		template< angle_unit U2, typename T2 > angle_<U, T>& operator=( const angle_<U2, T2>& a ) { value = angle_converter<U2, U>::convert( T( a.value ) ); return *this; }

		/// return degree copy
		angle_< angle_unit::degrees, T > degree() const { return angle_< angle_unit::degrees, T >( angle_converter< U, angle_unit::degrees >::convert( value ) ); }
		angle_< angle_unit::radians, T > radian() const { return angle_< angle_unit::radians, T >( angle_converter< U, angle_unit::radians >::convert( value ) ); }

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
		angle_ operator-() const { return angle_( -value ); }

		/// comparison operators
		bool operator>( const angle_& other ) const { return value > other.value; }
		bool operator<( const angle_& other ) const { return value < other.value; }
		bool operator>=( const angle_& other ) const { return value >= other.value; }
		bool operator<=( const angle_& other ) const { return value <= other.value; }
		bool operator==( const angle_& other ) const { return value == other.value; }
		bool operator!=( const angle_& other ) const { return value != other.value; }

		/// explicit type conversion (use with care)
		explicit operator float() const { return static_cast<float>( value ); }
		explicit operator double() const { return static_cast<double>( value ); }

		/// actual value
		T value;
	};

	// alias names
	template < typename T > using radian_ = angle_< angle_unit::radians, T >;
	template < typename T > using degree_ = angle_< angle_unit::degrees, T >;
	using radianf = radian_< float >;
	using radiand = radian_< double >;
	using degreef = degree_< float >;
	using degreed = degree_< double >;

	template< typename T > angle_< angle_unit::radians, T > rad( T rad ) { return angle_<angle_unit::radians, T>( rad ); }
	template< typename T > angle_< angle_unit::degrees, T > deg( T deg ) { return angle_<angle_unit::degrees, T>( deg ); }

	inline namespace literals {
		/// user-defined literals, add 'using xo::literals' to access them outside the xo namespace
		inline degreef operator"" _degf( long double v ) { return degreef( v ); }
		inline degreef operator"" _degf( unsigned long long int v ) { return degreef( v ); }
		inline radianf operator"" _radf( long double v ) { return radianf( v ); }
		inline radianf operator"" _radf( unsigned long long int v ) { return radianf( v ); }
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

	///// streaming
	//template< angle_unit U, typename T > std::ostream& operator<<( std::ostream& str, const angle_<U, T>& a ) { return ( str << a.value ); }
	//template< angle_unit U, typename T > std::istream& operator>>( std::istream& str, angle_<U, T>& a ) { return ( str >> a.value ); }
	//template< angle_unit U, typename T > char_stream& operator>>( char_stream& str, angle_<U, T>& a ) { return ( str >> a.value ); }

	template< angle_unit U, typename T > string to_str( const angle_<U, T>& a ) { return to_str( a.value ); }
	template< angle_unit U, typename T > angle_<U, T> from_str( const string& str, const angle_<U, T>& def_value ) { return angle_<U, T>( from_str( str, def_value.value ) ); }
}
