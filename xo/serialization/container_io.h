#pragma once

#include <fstream>
#include <utility>
#include <vector>
#include "xo/system/error_code.h"

namespace xo
{
	template< typename T1, typename T2 >
	std::istream& operator>>( std::istream& is, std::pair<T1, T2>& p ) {
		return is >> p.first >> p.second;
	}

	template< typename T >
	std::istream& operator>>( std::istream& is, std::vector<T>& v ) {
		while ( is.good() ) {
			T element;
			is >> element;
			if ( is.good() )
				v.emplace_back( element );
		}
		return is;
	}

	template< typename T >
	T read_data( const xo::path& file, xo::error_code* ec = nullptr ) {
		T result;
		auto is = std::ifstream( file.str() );
		if ( is.good() ) {
			is >> result;
			if ( !is.eof() )
				set_error_or_throw( ec, "Not all data was read from " + file.str() );
			if ( std::empty( result ) )
				set_error_or_throw( ec, "No data was read from " + file.str() );
		}
		else set_error_or_throw( ec, "Could not open " + file.str() );
		return result;
	}
}
