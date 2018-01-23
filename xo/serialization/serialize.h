#pragma once

#include "xo/container/prop_node.h"
#include <fstream>

namespace xo
{
	enum class file_format { unknown, xml, ini, prop };

	struct prop_node_serializer {
		explicit prop_node_serializer( file_format ff, const prop_node& pn, error_code* ec = nullptr ) : ff_( ff ), pn_( pn ), ec_( ec ) {}
		file_format ff_;
		const prop_node& pn_;
		error_code* ec_;
	};

	struct prop_node_deserializer {
		explicit prop_node_deserializer( file_format ff, prop_node& pn, error_code* ec = nullptr ) : ff_( ff ), pn_( pn ), ec_( ec ) {}
		file_format ff_;
		prop_node& pn_;
		error_code* ec_;
	};

	XO_API std::ostream& operator<<( std::ostream& str, prop_node_serializer& p );
	XO_API std::istream& operator>>( std::istream& str, prop_node_deserializer& p );

	XO_API file_format detect_file_format( const path& filename );

	XO_API prop_node load_file( const path& filename, file_format ff, error_code* ec = nullptr );
	XO_API prop_node load_file( const path& filename, error_code* ec = nullptr );
	inline prop_node load_xml( const path& filename, error_code* ec = nullptr ) { return load_file( filename, file_format::xml, ec ); }
	inline prop_node load_prop( const path& filename, error_code* ec = nullptr ) { return load_file( filename, file_format::prop, ec ); }
	XO_API prop_node parse_prop( const char* str, error_code* ec = nullptr );
	inline prop_node load_ini( const path& filename, error_code* ec = nullptr ) { return load_file( filename, file_format::ini, ec ); }
	XO_API prop_node load_file_with_include( const path& filename, const string& include_directive = "INCLUDE" );

	XO_API void save_file( const prop_node& pn, const path& filename, file_format ff, error_code* ec = nullptr );
	inline void save_xml( const prop_node& pn, const path& filename, error_code* ec = nullptr ) { save_file( pn, filename, file_format::xml, ec ); }
	inline void save_prop( const prop_node& pn, const path& filename, error_code* ec = nullptr ) { save_file( pn, filename, file_format::prop, ec ); }
	inline void save_ini( const prop_node& pn, const path& filename, error_code* ec = nullptr ) { save_file( pn, filename, file_format::ini, ec ); };
}
