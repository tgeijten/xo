#include "prop_node_serializer_zml.h"

#include <fstream>

namespace xo
{
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
				str.get_line();
			}
			else if ( t == "/*" ) // could be a comment
			{
				if ( !str.seek_past( "*/" ) )
					return zml_error( str, ec, "Could not find matching */" ), string();
			}
			else return trim_str( t );
		}
	}

	void read_zml_layer( char_stream& str, prop_node& parent, const string& close, error_code* ec, const path& folder )
	{
		prop_node merge_pn;
		for ( string t = get_zml_token( str, ec ); t != close; t = get_zml_token( str, ec ) )
		{
			if ( t.empty() ) // check if the stream has ended while expecting a close tag
				return zml_error( str, ec, "Unexpected end of stream" );

			// check directive statement
			if ( t[ 0 ] == '#' )
			{
				if ( t == "#include" )
					parent.append( load_zml( folder / path( get_zml_token( str, ec ) ), ec ) );
				else if ( t == "#merge" )
					merge_pn.merge( load_zml( folder / path( get_zml_token( str, ec ) ), ec ) );
				else return zml_error( str, ec, "Unknown directive: " + t );
			}
			else
			{
				// check if we're reading an array or if this is a label
				if ( close != "]" )
				{
					// read label
					if ( !isalpha( t[ 0 ] ) )
						return zml_error( str, ec, "Invalid label " + t );
					parent.push_back( t );

					// read =
					t = get_zml_token( str, ec );
					if ( t == "=" )
						t = get_zml_token( str, ec );
					else if ( t != "{" && t != "[" )
						return zml_error( str, ec, "Expected '=', '{' or '['" );
				}
				else parent.push_back( "" ); // add array child

											 // read value
				if ( t == "{" ) // new group
					read_zml_layer( str, parent.back().second, "}", ec, folder );
				else if ( t == "[" ) // new array
					read_zml_layer( str, parent.back().second, "]", ec, folder );
				else // just a value
					parent.back().second.set_value( std::move( t ) );
			}
		}

		// add children from #merge directive
		parent.merge( merge_pn, false );
	}

	prop_node parse_zml( char_stream& str, error_code* ec, const path& folder )
	{
		str.set_operators( { "=", "{", "}", "[", "]", ";", "//", "/*", "*/" } );
		str.set_delimiter_chars( " \n\r\t\v" );
		str.set_quotation_chars( "\"" );

		prop_node root;
		read_zml_layer( str, root, "", ec, folder );
		return root;
	}

	prop_node parse_zml( const char* str, error_code* ec )
	{
		return parse_zml( char_stream( str ), ec, path() );
	}

	void write_zml_node( std::ostream& str, const string& label, const prop_node& pn, int level )
	{
		auto indent = string( level, '\t' );

		// = and value
		if ( !label.empty() )
		{
			// TODO: this does not work when arrays contain objects!
			str << indent << try_quoted( label, ";{}[]#" );
			str << " = " << ( pn.empty() ? "\"\"" : try_quoted( pn.get_value(), ";{}[]#" ) );
		}

		// check if this is an array
		bool is_array = find_if( pn, [&]( const prop_node::pair_t& n ) { return !n.first.empty(); } ) == pn.end();
		auto depth = pn.count_layers();
		string separate = depth == 1 ? " " : "\n" + indent;

		if ( depth == 0 )
		{
			str << std::endl;
		}
		else if ( is_array && depth > 0 )
		{
			// TODO: this does not work when arrays contain objects!
			str << "[" << separate;
			for ( auto& node : pn )
				str << try_quoted( node.second.get_value(), ";{}[]" ) << separate;
			str << "]" << std::endl;
		}
		else if ( depth == 1 && pn.size() <= 4 )
		{
			str << "{" << separate;
			for ( auto& node : pn )
				str << node.first << " = " << try_quoted( node.second.get_value(), ";{}[]" ) << separate;
			str << "}" << std::endl;
		}
		else
		{
			// multi-line children
			str << "{" << std::endl;
			for ( auto& node : pn )
				write_zml_node( str, node.first, node.second, level + 1 );
			str << indent << "}" << std::endl;
		}
	}

	void write_zml_node_concise( std::ostream& str, const string& label, const prop_node& pn )
	{
		if ( !label.empty() )
		{
			// TODO: this does not work when arrays contain objects!
			str << try_quoted( label, ";{}[]#" );
			str << "=" << try_quoted( pn.get_value(), ";{}[]#" );
		}

		if ( pn.size() > 0 )
		{
			// check if this is an array
			if ( find_if( pn, [&]( const prop_node::pair_t& n ) { return !n.first.empty(); } ) == pn.end() )
			{
				str << "[ ";
				for ( auto& node : pn )
					str << try_quoted( node.second.get_value(), ";{}[]" ) << ' ';
				str << "]" << std::endl;
			}
			else
			{
				// multi-line children
				str << "{ ";
				for ( auto& node : pn )
				{
					write_zml_node_concise( str, node.first, node.second );
					str << ' ';
				}
				str << "}";
			}
		}
	}

	std::istream& prop_node_serializer_zml::read_stream( std::istream& str )
	{
		xo_assert( read_pn_ );
		// TODO: more efficient. parser should be able to take any stream type.
		*read_pn_ = parse_zml( char_stream( string( std::istreambuf_iterator<char>( str ), {} ) ), ec_, file_folder_ );
		return str;
	}

	std::ostream& prop_node_serializer_zml::write_stream( std::ostream& str )
	{
		xo_assert( write_pn_ );
		for ( auto& node : *write_pn_ )
			write_zml_node( str, node.first, node.second, 0 );
		return str;
	}

	std::ostream& prop_node_serializer_zml_concise::write_stream( std::ostream& str )
	{
		xo_assert( write_pn_ );
		for ( auto& node : *write_pn_ )
		{
			write_zml_node_concise( str, node.first, node.second );
			str << ' ';
		}
		return str;
	}

	XO_API prop_node load_zml( const path& filename, error_code* ec, path parent_folder )
	{
		return parse_zml( char_stream( filename ), ec, filename.parent_path() );
	}
}
