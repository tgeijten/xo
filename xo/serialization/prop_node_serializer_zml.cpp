#include "prop_node_serializer_zml.h"

#include <fstream>
#include "char_stream.h"

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

	void read_zml_layer( char_stream& str, prop_node& parent, const string& close, error_code* ec, const path& folder, str_replace_vec& vars )
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
				else if ( t == "#set" )
				{
					auto key = '(' + get_zml_token( str, ec ) + ')'; // do this first because order of evaluation
					vars.emplace_back( std::move( key ), get_zml_token( str, ec ) );
				}
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

					replace_all( t, vars );
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
					read_zml_layer( str, parent.back().second, "}", ec, folder, vars );
				else if ( t == "[" ) // new array
					read_zml_layer( str, parent.back().second, "]", ec, folder, vars );
				else // value
				{
					replace_all( t, vars );
					parent.back().second.set_value( std::move( t ) );
				}
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
		str_replace_vec defines;
		read_zml_layer( str, root, "", ec, folder, defines );
		return root;
	}

	prop_node parse_zml( const char* str, error_code* ec )
	{
		char_stream stream( str );
		return parse_zml( stream, ec, path() );
	}

	void write_zml_kvp( std::ostream& str, const string& label, const prop_node& pn, const char* equals_str = " = " )
	{
		bool show_label = !label.empty();
		bool show_value = !pn.get_value().empty() || pn.size() == 0;
		if ( show_label )
			str << try_quoted( label, ";{}[]#" );
		if ( show_label && show_value )
			str << equals_str;
		if ( show_value )
			str << try_quoted( pn.get_value(), ";{}[]#" );
	}

	void write_zml_node( std::ostream& str, const string& label, const prop_node& pn, int level )
	{
		if ( level > 0 )
			str << string( level - 1, '\t' );

		write_zml_kvp( str, label, pn, " = " );

		if ( pn.size() > 0 )
		{
			// check if this is an array
			bool is_array = find_if( pn, [&]( const prop_node::pair_t& n ) { return !n.first.empty(); } ) == pn.end();
			auto depth = pn.count_layers();

			if ( depth == 1 && pn.size() <= 4 )
			{
				// single line children
				if ( level > 0 ) str << ( is_array ? " [ " : " { " );
				for ( auto it = pn.begin(); it != pn.end(); ++it )
				{
					if ( it != pn.begin() ) str << " ";
					write_zml_node( str, it->first, it->second, 0 );
				}
				if ( level > 0 ) str << ( is_array ? " ]" : " }" );
			}
			else
			{
				// multi-line children
				if ( level > 0 ) str << ( is_array ? " [" : " {" ) << std::endl;
				for ( auto it = pn.begin(); it != pn.end(); ++it )
				{
					write_zml_node( str, it->first, it->second, level + 1 );
					str << std::endl;
				}
				if ( level > 0 ) str << string( level - 1, '\t' ) << ( is_array ? "]" : "}" );
			}
		}
	}

	void write_zml_node_concise( std::ostream& str, const string& label, const prop_node& pn, int level )
	{
		write_zml_kvp( str, label, pn, "=" );
		if ( pn.size() > 0 )
		{
			// add children, either array or group
			bool is_array = find_if( pn, [&]( const prop_node::pair_t& n ) { return !n.first.empty(); } ) == pn.end();
			if ( level > 0 ) str << ( is_array ? '[' : '{' );
			for ( auto it = pn.begin(); it != pn.end(); ++it )
			{
				if ( it != pn.begin() ) str << " ";
				write_zml_node_concise( str, it->first, it->second, level + 1 );
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
		write_zml_node( str, "", *write_pn_, 0 );
		return str;
	}

	std::ostream& prop_node_serializer_zml_concise::write_stream( std::ostream& str ) const
	{
		xo_assert( write_pn_ );
		write_zml_node_concise( str, "", *write_pn_, 0 );
		return str;
	}

	XO_API prop_node load_zml( const path& filename, error_code* ec, path parent_folder )
	{
		char_stream stream( filename );
		return parse_zml( stream, ec, filename.parent_path() );
	}
}
