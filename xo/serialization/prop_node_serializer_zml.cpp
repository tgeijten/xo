#include "prop_node_serializer_zml.h"

#include <fstream>

#include "xo/serialization/char_stream.h"
#include "xo/string/string_tools.h"
#include "xo/container/prop_node.h"
#include "xo/container/container_tools.h"

namespace xo
{
	using str_replace_vec = std::vector< std::pair< string, string > >;
	void replace_all( string& s, const str_replace_vec& rvec )
	{
		for ( auto& r : rvec )
			replace_str( s, r.first, r.second );
	}

	void zml_error( const char_stream& str, error_code* ec, const string& message )
	{
		set_error_or_throw( ec, stringf( "Error parsing line %d: ", str.line_number() ) + message );
	}

	string get_zml_token( char_stream& str, error_code* ec )
	{
		while ( true )
		{
			string t = str.get_token();
			if ( t == ";" || t == "#" || t == "//" ) // single line comment: skip rest of line
			{
				if ( t == "#" && str.peekc() == '{' ) // multiline comment
					if ( !str.seek_past( "#}" ) )
						return zml_error( str, ec, "Could not find matching '#}'" ), string();

				str.get_line();
			}
			else if ( t == "/*" ) // multiline comment
			{
				if ( !str.seek_past( "*/" ) )
					return zml_error( str, ec, "Could not find matching '*/'" ), string();
			}
			else return t;
		}
	}

	void read_zml_layer( char_stream& str, prop_node& parent, const string& close, error_code* ec, const path& folder, const prop_node& root )
	{
		prop_node merge_pn;
		for ( string t = get_zml_token( str, ec ); t != close; t = get_zml_token( str, ec ) )
		{
			if ( t.empty() ) // check if the stream has ended while expecting a close tag
				return zml_error( str, ec, "Unexpected end of stream" );

			if ( t[ 0 ] == '#' || t == "<<" ) // check directive statement
			{
				auto filename = path( get_zml_token( str, ec ) );
				if ( t == "#include" || t == "<<" )
					parent.append( load_zml( folder / filename, ec ) );
				else if ( t == "#merge" )
					merge_pn.merge( load_zml( folder / path( get_zml_token( str, ec ) ), ec ) );
				else return zml_error( str, ec, "Unknown directive: " + t );

				if ( t == "<<" && get_zml_token( str, ec ) != ">>" )
					zml_error( str, ec, "Could not find matching '>>'" );
			}
			else
			{
				// check if we're reading an array or if this is a label
				if ( close != "]" )
				{
					if ( t != "{" && t != "[" )
					{
						// read label
						if ( !isalpha( t[ 0 ] ) )
							return zml_error( str, ec, "Invalid label " + t );
						parent.push_back( t );

						// read = or :
						t = get_zml_token( str, ec );
						if ( t == "=" || t == ": " )
							t = get_zml_token( str, ec );
						else if ( t != "{" && t != "[" )
							return zml_error( str, ec, "Expected '=', ':', '{' or '['" );
					}
				}
				else parent.push_back( "" ); // add array child

				// parse value element after
				if ( t == "{" ) // new group
					read_zml_layer( str, parent.back().second, "}", ec, folder, root );
				else if ( t == "[" ) // new array
					read_zml_layer( str, parent.back().second, "]", ec, folder, root );
				else if ( str_begins_with( t, "@" ) )
				{
					if ( auto ref_pn = root.try_get_query( t.substr( 1 ) ) )
						parent.back().second.set( *ref_pn );
					else return zml_error( str, ec, "Could not find " + t );
				}
				else
				{
					parent.back().second.set_value( std::move( t ) );
				}
			}
		}

		// add children from #merge directive
		parent.merge( merge_pn, false );
	}

	prop_node parse_zml( char_stream& str, error_code* ec, const path& folder )
	{
		str.set_operators( { "=", ": ", "{", "}", "[", "]", ";", "//", "/*", "*/" } );
		str.set_delimiter_chars( " \n\r\t\v" );
		str.set_quotation_chars( "\"'" );

		prop_node root;
		read_zml_layer( str, root, "", ec, folder, root );
		return root;
	}

	prop_node parse_zml( const char* str, error_code* ec )
	{
		char_stream stream( str );
		return parse_zml( stream, ec, path() );
	}

	void write_zml_kvp( std::ostream& str, const string& label, const prop_node& pn, const char* equals_str, bool inside_array )
	{
		bool show_label = !label.empty();
		bool show_value = !pn.get_value().empty() || pn.size() == 0;
		xo_error_if( !show_label && show_value && !inside_array, "Value without label outside array" );
		if ( show_label )
			str << try_quoted( label, ";{}[]#" );
		if ( show_label && show_value )
			str << equals_str;
		if ( show_value )
			str << try_quoted( pn.get_value(), ";{}[]#" );
	}

	void write_zml_node( std::ostream& str, const string& label, const prop_node& pn, int level, bool inside_array )
	{
		if ( level > 0 )
			str << string( level - 1, '\t' );

		write_zml_kvp( str, label, pn, " = ", inside_array );

		if ( pn.size() > 0 )
		{
			// check if this is an array
			bool is_array = find_if( pn, [&]( const prop_node::pair_t& n ) { return !n.first.empty(); } ) == pn.end();
			auto depth = pn.count_layers();

			if ( depth == 1 && pn.size() <= 3 )
			{
				// single line children
				if ( level > 0 ) str << ( is_array ? " [ " : " { " );
				for ( auto it = pn.begin(); it != pn.end(); ++it )
				{
					if ( it != pn.begin() ) str << " ";
					write_zml_node( str, it->first, it->second, 0, is_array );
				}
				if ( level > 0 ) str << ( is_array ? " ]" : " }" );
			}
			else
			{
				// multi-line children
				if ( level > 0 ) str << ( is_array ? " [" : " {" ) << std::endl;
				for ( auto it = pn.begin(); it != pn.end(); ++it )
				{
					write_zml_node( str, it->first, it->second, level + 1, is_array );
					str << std::endl;
				}
				if ( level > 0 ) str << string( level - 1, '\t' ) << ( is_array ? "]" : "}" );
			}
		}
	}

	void write_zml_node_concise( std::ostream& str, const string& label, const prop_node& pn, int level, bool inside_array )
	{
		write_zml_kvp( str, label, pn, "=", inside_array );
		if ( pn.size() > 0 )
		{
			// add children, either array or group
			bool is_array = find_if( pn, [&]( const prop_node::pair_t& n ) { return !n.first.empty(); } ) == pn.end();
			if ( level > 0 ) str << ( is_array ? '[' : '{' );
			for ( auto it = pn.begin(); it != pn.end(); ++it )
			{
				if ( it != pn.begin() ) str << " ";
				write_zml_node_concise( str, it->first, it->second, level + 1, inside_array );
			}
			if ( level > 0 ) str << ( is_array ? ']' : '}' );
		}
	}

	std::istream& prop_node_serializer_zml::read_stream( std::istream& str )
	{
		xo_assert( read_pn_ );
		// TODO: more efficient. parser should be able to take any stream type.
		char_stream stream( string( std::istreambuf_iterator<char>( str ), {} ) );
		*read_pn_ = parse_zml( stream, ec_, file_folder_ );
		return str;
	}

	std::ostream& prop_node_serializer_zml::write_stream( std::ostream& str ) const
	{
		xo_assert( write_pn_ );
		write_zml_node( str, "", *write_pn_, 0, false );
		return str;
	}

	std::ostream& prop_node_serializer_zml_concise::write_stream( std::ostream& str ) const
	{
		xo_assert( write_pn_ );
		write_zml_node_concise( str, "", *write_pn_, 0, false );
		return str;
	}

	XO_API prop_node load_zml( const path& filename, error_code* ec, path parent_folder )
	{
		char_stream stream( filename );
		return parse_zml( stream, ec, filename.parent_path() );
	}
}
