#pragma once

#include "xo/string/string_type.h"
#include <sstream>
#include <string>

namespace xo
{
	template< typename T, typename E = void > struct string_cast {
		static T from( const string& s ) { T value = T(); std::stringstream str( s ); str >> value; return value; }
		static string to( const T& value ) { std::ostringstream str; str << value; return str.str(); }
	};

	template<> struct string_cast< float, void > {
		static float from( const string& s ) { return std::stof( s ); }
		static string to( float value ) { return std::to_string( value ); }
	};

	template<> struct string_cast< double, void > {
		static double from( const string& s ) { return std::stod( s ); }
		static string to( double value ) { return std::to_string( value ); }
	};

	template<> struct string_cast< long, void > {
		static long from( const string& s ) { return std::stol( s ); }
		static string to( long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast< long long, void > {
		static long long from( const string& s ) { return std::stoll( s ); }
		static string to( long long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast< unsigned long, void > {
		static unsigned long from( const string& s ) { return std::stoul( s ); }
		static string to( unsigned long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast< unsigned long long, void > {
		static unsigned long long from( const string& s ) { return std::stoull( s ); }
		static string to( unsigned long long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast< const char*, void > {
		static const char* from( const string& s ) { return s.c_str(); }
		static string to( const char* value ) { return string( value ); }
	};

	template<> struct string_cast< string, void > {
		static string from( const string& s ) { return s; }
		static string to( const string& value ) { return value; }
	};

	template< typename T > struct string_cast< T, typename std::enable_if< std::is_enum< T >::value >::type > {
		static T from( const string& s ) { return static_cast< T >( string_cast< int >::from( s ) ); }
		static string to( const T& v ) { return string_cast< int >::to( static_cast< int >( v ) ); }
	};

	template< typename T > bool string_is( const string& s ) {
		try { T result = string_cast<T>::from( s ); return true; }
		catch( std::exception& e ) { return false; }
	}
}
