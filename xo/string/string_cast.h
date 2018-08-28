#pragma once

#include "xo/system/assert.h"
#include "xo/string/string_type.h"
#include <sstream>

namespace xo
{
	/// convert any type to string
	template< typename T > string to_str( const T& value ) { return string_cast<T>::to( value ); }

	template< typename E > string to_str( std::initializer_list< E > list, const string& delim = " " ) {
		string s;
		for ( auto it = list.begin(); it != list.end(); ++it ) {
			if ( it != list.begin() ) s += delim;
			s += to_str( *it );
		}
		return s;
	}

	/// convert string to any type
	template< typename T > T from_str( const string& s ) { return string_cast<T>::from( s ); }

	template< typename T, typename E = void > struct string_cast {
		static T from( const string& s ) { T value = T(); std::stringstream str( s ); str >> value; return value; }
		static string to( const T& value ) { std::ostringstream str; str << value; return str.str(); }
	};

	template<> struct string_cast< float, void > {
		static float from( const string& s ) { return std::stof( s ); }
		static string to( float value ) { char buf[ 32 ]; sprintf_s( buf, sizeof( buf ), "%g", value ); return string( buf ); }
	};

	template<> struct string_cast< double, void > {
		static double from( const string& s ) { return std::stod( s ); }
		static string to( double value ) { char buf[ 32 ]; sprintf_s( buf, sizeof( buf ), "%g", value ); return string( buf ); }
	};

	template<> struct string_cast<int, void> {
		static long from( const string& s ) { return std::stoi( s ); }
		static string to( long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast<unsigned int, void> {
		static unsigned int from( const string& s ) { return static_cast<unsigned int>( std::stoul( s ) ); } // this needs a cast, because C++ has no stoui
		static string to( long long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast<long, void> {
		static long from( const string& s ) { return std::stol( s ); }
		static string to( long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast<unsigned long, void> {
		static unsigned long from( const string& s ) { return std::stoul( s ); }
		static string to( unsigned long value ) { return std::to_string( value ); }
	};

	template<> struct string_cast< long long, void > {
		static long long from( const string& s ) { return std::stoll( s ); }
		static string to( long long value ) { return std::to_string( value ); }
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

	template< typename T > bool can_string_cast( const string& s ) {
		try { T result = string_cast<T>::from( s ); return true; }
		catch ( std::exception& e ) { return false; }
	}
}
