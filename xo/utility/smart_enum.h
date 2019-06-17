#pragma once

#include <cstring>
#include <string>
#include <iostream>

namespace smart_enum_detail
{
	constexpr const char* sep_chars = " ,\t\n\r";
	template< typename E > bool from_str( const std::string& str, E& e, const char* va_args ) {
		for ( int i = 0; *va_args; ++i ) {
			if ( std::strncmp( str.c_str(), va_args, str.size() ) == 0 ) { e = E( i ); return true; }
			va_args += std::strcspn( va_args, sep_chars );
			va_args += std::strspn( va_args, sep_chars );
		}
		return false;
	} 
	inline std::string to_str( int e, const char* va_args ) {
		while ( e-- > 0 && *va_args ) {
			va_args += std::strcspn( va_args, sep_chars );
			va_args += std::strspn( va_args, sep_chars );
		}
		return std::string( va_args, std::strcspn( va_args, sep_chars ) );
	}
}

#define XO_SMART_ENUM_DETAIL_DEFINE_FUNCTIONS( E, VA_STR ) \
inline std::string to_str( E e ) { return ::smart_enum_detail::to_str( int( e ), VA_STR ); } \
inline bool from_str( const std::string& str, E& e ) { return ::smart_enum_detail::from_str( str, e, VA_STR ); } \
inline std::ostream& operator<<( std::ostream& ostr, E e ) { return ostr << to_str( e ); } \
inline std::istream& operator>>( std::istream& istr, E& e ) { std::string s; istr >> s; if ( !from_str( s, e ) ) istr.setstate( std::ios::failbit ); return istr; }

#define xo_smart_enum( E, ... ) \
enum E { __VA_ARGS__ }; \
XO_SMART_ENUM_DETAIL_DEFINE_FUNCTIONS( E, #__VA_ARGS__ )

#define xo_smart_enum_type( E, T, ... ) \
enum E : T { __VA_ARGS__ }; \
XO_SMART_ENUM_DETAIL_DEFINE_FUNCTIONS( E, #__VA_ARGS__ )

#define xo_smart_enum_class( E, ... ) \
enum class E { __VA_ARGS__ }; \
XO_SMART_ENUM_DETAIL_DEFINE_FUNCTIONS( E, #__VA_ARGS__ )

#define xo_smart_enum_class_type( E, T, ... ) \
enum class E : T { __VA_ARGS__ }; \
XO_SMART_ENUM_DETAIL_DEFINE_FUNCTIONS( E, #__VA_ARGS__ )
