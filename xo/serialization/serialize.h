#pragma once

#include "xo/container/prop_node.h"
#include <fstream>

namespace xo
{
	enum class file_format { unknown, xml, ini, pn };

	template< file_format FF > struct prop_node_serializer {
		explicit prop_node_serializer( const prop_node& pn, error_code* ec = nullptr ) : pn( pn ), ec( ec ) {}
		const prop_node& pn;
		error_code* ec;
	};

	template< file_format FF > struct prop_node_deserializer {
		explicit prop_node_deserializer( prop_node& pn, error_code* ec = nullptr ) : pn( pn ), ec( ec ) {}
		prop_node& pn;
		error_code* ec;
	};

	XO_API std::ostream& operator<<( std::ostream& str, xo::prop_node_serializer< xo::file_format::xml > p );
	XO_API std::istream& operator>>( std::istream& str, xo::prop_node_deserializer< xo::file_format::xml > p );
	XO_API std::ostream& operator<<( std::ostream& str, xo::prop_node_serializer< xo::file_format::pn > p );
	XO_API std::istream& operator>>( std::istream& str, xo::prop_node_deserializer< xo::file_format::pn > p );
	XO_API std::ostream& operator<<( std::ostream& str, xo::prop_node_serializer< xo::file_format::ini > p );
	XO_API std::istream& operator>>( std::istream& str, xo::prop_node_deserializer< xo::file_format::ini > p );

	template< file_format FF >
	prop_node load_file_format( const path& filename, error_code* ec = nullptr )
	{
		prop_node pn;
		std::ifstream str( filename.c_str() ); // TODO: use char_stream
		if ( str ) str >> prop_node_deserializer< FF >( pn, ec );
		else set_error_or_throw( ec, "Could not open file: " + filename.string() );
		return pn;
	}

	template< file_format FF >
	void save_file( const prop_node& pn, const path& filename, error_code* ec = nullptr )
	{
		std::ofstream str( filename.c_str() );
		if ( str ) str << prop_node_serializer< FF >( pn, ec );
		else set_error_or_throw( ec, "Could not save to file: " + filename.string() );
	}

	/// load file, autodetect type
	XO_API prop_node load_file( const path& filename, error_code* ec = nullptr );

	/// load/save contents from xml
	inline prop_node load_xml( const path& filename, error_code* ec = nullptr ) { return load_file_format< file_format::xml >( filename, ec ); }
	inline void save_xml( const prop_node& pn, const path& filename, error_code* ec = nullptr ) { save_file< file_format::xml >( pn, filename, ec ); }

	/// load/save contents from prop
	inline prop_node load_prop( const path& filename, error_code* ec = nullptr ) { return load_file_format< file_format::pn >( filename, ec ); }
	inline void save_prop( const prop_node& pn, const path& filename, error_code* ec = nullptr ) { save_file< file_format::pn >( pn, filename, ec ); }
	XO_API prop_node parse_prop( const char* str, error_code* ec = nullptr );

	/// load contents from prop
	inline prop_node load_ini( const path& filename, error_code* ec = nullptr )  { return load_file_format< file_format::ini >( filename, ec ); }
	inline void save_ini( const prop_node& pn, const path& filename, error_code* ec = nullptr ) { save_file< file_format::ini >( pn, filename, ec ); };
}
