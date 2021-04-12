#pragma once

#include "xo/geometry/vec3_type.h"
#include "xo/geometry/angle_type.h"
#include "xo/string/string_cast.h"
#include <cmath>

namespace xo
{
	/// sin/cos/tan
	template< angle_unit U, typename T > T sin( const angle_<U, T>& a ) { return std::sin( a.rad_value() ); }
	template< angle_unit U, typename T > T cos( const angle_<U, T>& a ) { return std::cos( a.rad_value() ); }
	template< angle_unit U, typename T > T tan( const angle_<U, T>& a ) { return std::tan( a.rad_value() ); }
	template< angle_unit U, typename T > T asin( const angle_<U, T>& a ) { return std::asin( a.rad_value() ); }
	template< angle_unit U, typename T > T acos( const angle_<U, T>& a ) { return std::acos( a.rad_value() ); }
	template< angle_unit U, typename T > T atan( const angle_<U, T>& a ) { return std::atan( a.rad_value() ); }

	/// Make vec3 from polar angles (radius, inclination and azimuth)
	template< angle_unit U, typename T > vec3_<T> vec3_from_polar( T radius, angle_<U, T> inc, angle_<U, T> azi ) {
		return vec3_<T>( radius * sin( inc ) * sin( azi ), radius * cos( inc ), radius * sin( inc ) * cos( azi ) );
	}

	/// convert angle to string
	template< angle_unit U, typename T > string to_str( const angle_<U, T>& a ) {
		return to_str( a.value );
	}

	/// convert angle from string
	template< angle_unit U, typename T > bool from_str( const string& str, angle_<U, T>& v ) {
		return from_str( str, v.value );
	}

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
}
