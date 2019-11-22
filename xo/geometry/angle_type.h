#pragma once

#include "xo/xo_types.h"
#include "xo/utility/arithmetic.h"
#include "xo/geometry/vec3_type.h"

namespace xo
{
	/// base angle_ definition
	template< angle_unit U, typename T >
	struct angle_ {};

	/// partial specialization for degrees
	template< typename T >
	struct angle_< angle_unit::degrees, T > : arithmetic< T, angle_<angle_unit::degrees, T>>
	{
		/// default constructor
		constexpr angle_() : arithmetic< T, angle_<angle_unit::degrees, T>>( T() ) {}

		/// copy / conversion constructor
		template< angle_unit U, typename T2 > explicit constexpr angle_( const angle_<U, T2>& a )
		    : arithmetic< T, angle_<angle_unit::degrees, T>>( T( a.deg_value() ) ) {}

		/// value constructor
		template< typename T2 > explicit constexpr angle_( const T2& v )
		    : arithmetic< T, angle_<angle_unit::degrees, T>>( T( v ) ) {}

		/// copy assignment
		angle_& operator=( const angle_& a ) { this->value = a.value; return *this; }

		/// get rad / deg value
		constexpr const T& deg_value() const { return this->value; }
		constexpr T rad_value() const { return T( 0.017453292519943295769L ) * this->value; }

		/// explicit cast to value type
		explicit operator T() const { return this->value; }
	};

	/// partial specialization for radians
	template< typename T >
	struct angle_< angle_unit::radians, T > : arithmetic< T, angle_<angle_unit::radians, T> >
	{
		/// default constructor
		constexpr angle_() : arithmetic< T, angle_<angle_unit::radians, T> >( T() ) {}

		/// copy / conversion constructor
		template< angle_unit U, typename T2 > explicit constexpr angle_( const angle_<U, T2>& a )
		    : arithmetic< T, angle_<angle_unit::radians, T> >( T( a.rad_value() ) ) {}

		/// value constructor
		template< typename T2 > explicit constexpr angle_( const T2& v )
		    : arithmetic< T, angle_<angle_unit::radians, T> >( T( v ) ) {}

		/// copy assignment
		angle_& operator=( const angle_& a ) { this->value = a.value; return *this; }

		/// get rad / deg value
		constexpr T deg_value() const { return T( 57.295779513082320877L ) * this->value; }
		constexpr const T& rad_value() const { return this->value; }

		/// explicit cast to value type
		explicit operator T() const { return this->value; }
	};

	// alias names
	template < typename T > using radian_ = angle_<angle_unit::radians, T>;
	using radianf = radian_<float>;
	using radiand = radian_<double>;
	template < typename T > using degree_ = angle_<angle_unit::degrees, T>;
	using degreef = degree_<float>;
	using degreed = degree_<double>;

	/// alias for angle vector
	template< typename T > using vec3rad_ = vec3_< radian_<T> >;
	using vec3radf = vec3rad_< float >;
	using vec3radd = vec3rad_< double >;
	template< typename T > using vec3deg_ = vec3_< degree_<T> >;
	using vec3degf = vec3deg_< float >;
	using vec3degd = vec3deg_< double >;

	/// convert vector of angles to vector of values
	template< angle_unit U, typename T > vec3_<T> value( const vec3_< angle_<U, T> >& v ) {
		return vec3_<T>( v.x.value, v.y.value, v.z.value );
	}

	/// user-defined literals, add 'using namespace xo::literals' to access them outside the xo namespace
	inline namespace literals {
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
