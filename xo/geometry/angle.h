#pragma once

#include "xo/geometry/vec3_type.h"
#include "xo/geometry/angle_type.h"
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
	template< angle_unit U, typename T > vec3_<T> vec3_from_polar( T radius, angle_<U, T> inc, angle_<U, T> azi )
	{ return vec3_<T>( radius * sin( inc ) * sin( azi ), radius * cos( inc ), radius * sin( inc ) * cos( azi ) ); }
}
