#pragma once

#include "xo/system/platform.h"
#include "xo/system/assert.h"
#include "xo/string/string_type.h"

#include <type_traits>
#include <sstream> // #TODO: really should get rid of this header

namespace xo
{
	/// convert any type to string
	template< typename T > string to_str( const T& value );
	template< typename T > T from_str( const string& s, T default_value );

	// specializations
	template<> inline float from_str( const string& s, float default_value ) { char* p; auto v = std::strtof( s.c_str(), &p ); return p != s.c_str() ? v : default_value; }
	XO_API string to_str( const float& value );

	template<> inline double from_str( const string& s, double default_value ) { char* p; auto v = std::strtod( s.c_str(), &p ); return p != s.c_str() ? v : default_value; }
	XO_API string to_str( const double& value );

	template<> inline bool from_str( const string& s, bool default_value ) { return ( s == "1" || s == "true" ); }
	XO_API string to_str( const bool& value );

	template<> inline unsigned int from_str( const string& s, unsigned int default_value ) { char* p; auto v = (unsigned int)( std::strtol( s.c_str(), &p, 10 ) ); return p != s.c_str() ? v : default_value; }
	XO_API string to_str( const unsigned int& value );

	template<> inline long from_str( const string& s, long default_value ) { char* p; auto v = std::strtol( s.c_str(), &p, 10 ); return p != s.c_str() ? v : default_value; }
	XO_API string to_str( const long& value );

	template<> inline unsigned long from_str( const string& s, unsigned long default_value ) { char* p; auto v = std::strtoul( s.c_str(), &p, 10 ); return p != s.c_str() ? v : default_value; }
	XO_API string to_str( const unsigned long& value );

	template<> inline const char* from_str( const string& s, const char* default_value ) { return s.c_str(); }
	XO_API string to_str( const char* value );

	template<> inline string from_str( const string& s, string default_value ) { return s; }
	inline string to_str( string value ) { return value; }

	template< typename T, typename E = void > struct string_cast {
		static T from( const string& s ) { T value = T(); std::stringstream str( s ); str >> value; return value; }
		static string to( const T& value ) { std::ostringstream str; str << value; return str.str(); }
	};

	template< typename T > struct string_cast< T, typename std::enable_if< std::is_enum< T >::value >::type > {
		static T from( const string& s ) { return static_cast< T >( string_cast< int >::from( s ) ); }
		static string to( const T& v ) { return string_cast< int >::to( static_cast< int >( v ) ); }
	};

	/// make string out of container of elements
	template< typename C > string to_str( const C& c, const string& delim ) {
		string s;
		for ( auto it = std::begin( c ); it != std::end( c ); ++it ) {
			if ( it != c.begin() ) s += delim;
			s += to_str( *it );
		}
		return s;
	}

	template< typename T > string to_str( const T& value ) { return string_cast<T>::to( value ); }
	template< typename T > T from_str( const string& s, T default_value ) { return string_cast<T>::from( s ); }
}
