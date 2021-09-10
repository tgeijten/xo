#include "system_tools.h"

#ifdef XO_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <conio.h>
#	include <shlobj.h>
#	pragma warning( disable: 4996 )
#else
#   include <cxxabi.h>
#endif

#include "xo/string/string_tools.h"
#include "xo/system/log.h"

#include <fstream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <sstream>
#include "assert.h"

namespace xo
{
	version XO_VERSION = version( 0, 1, 0 );
	version get_xo_version() { return XO_VERSION; }

	XO_API char wait_for_key()
	{
#ifdef XO_COMP_MSVC
			return _getch();
#else
			return 0;
#endif
	}

	XO_API string get_date_time_str( const char* format )
	{
		auto in_time_t = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
		std::stringstream ss;
		ss << std::put_time( std::localtime( &in_time_t ), format );
		return ss.str();
	}

	XO_API void crash( const string& message )
	{
		if ( !message.empty() )
			log::critical( message );

		// crash!
		*(volatile int*)(0) = 123;
	}

	XO_API void sleep( int ms )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
	}

	XO_API string tidy_identifier( const string& id )
	{
		size_t pos = id.find_last_of( ".:" );
		pos = pos != string::npos ? pos + 1 : 0;
		while ( pos < id.size() && id[ pos ] == '_' )
			++pos;
		size_t end_pos = id.size();
		while ( end_pos > 0 && id[ end_pos - 1 ] == '_' )
			--end_pos;
		return id.substr( pos, end_pos - pos );
	}

	XO_API string tidy_type_name( const string& name0 )
	{
		std::string name = name0;
#ifndef XO_COMP_MSVC
		int status;
		char* cleanType = abi::__cxa_demangle( name.c_str(), 0, 0, &status );
		auto cleanStr = std::string( cleanType );
		free( cleanType );
		name = cleanStr;
		//return cleanStr;
#endif
		auto endpos = name.find_first_of( "<" );
		size_t pos = name.find_last_of( ": ", endpos );
		if ( pos != std::string::npos )
		{
			if ( endpos != std::string::npos )
				return name.substr( pos + 1, endpos - pos - 1 );
			else return name.substr( pos + 1 );
		}
		else return name;
	}

	string get_computer_name()
	{
#ifdef XO_COMP_MSVC
		char buf[ 256 ] = "";
		DWORD len = 256;
		if ( !GetComputerName( buf, &len ) )
			return "";
		else return buf;
#else
		return string( "" );
#endif
	}
}
