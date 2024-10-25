#include "prop_node_serializer_zml.h"

#include <fstream>
#include <sstream>

#include "xo/serialization/char_stream.h"
#include "xo/string/string_tools.h"
#include "xo/container/prop_node.h"
#include "xo/container/container_tools.h"
#include "xo/system/log.h"
#include "xo/filesystem/filesystem.h"
#include <algorithm>

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
			if ( t == "#" || t == "//" ) {
				// single line comment
				str.get_line(); 
			}
			else if ( t == "/*" ) {
				// multiline comment
				if ( !str.seek_past( "*/" ) )
					return zml_error( str, ec, "Multiline comment '/*' has no matching '*/'" ), string();
			}
			else return t;
		}
	}

	void read_zml_layer( char_stream& str, prop_node& parent, const string& close, error_code* ec, const path& folder, vector<path>* included_files, const prop_node& root, prop_node& macros )
	{
		// keep track of the number of macros so we can delete them at the end of this scope
		auto macro_count = macros.size();

		// iterate over all items in this scope
		for ( string t = get_zml_token( str, ec ); t != close; t = get_zml_token( str, ec ) )
		{
			if ( t.empty() ) // check if the stream has ended while expecting a close tag
				return zml_error( str, ec, "Unexpected end of stream" );

			if ( t == "<<" ) // check directive statement
			{
				auto filename = path( get_zml_token( str, ec ) );
				if ( get_zml_token( str, ec ) != ">>" )
					zml_error( str, ec, "'<<' has no find matching '>>'" );
				if ( filename.is_relative() )
					filename = folder / filename;
				parent.append( load_zml( filename, ec ) );
				if ( included_files )
					included_files->emplace_back( std::move( filename ) );
			}
			else
			{
				prop_node* next_parent = nullptr;

				// check if we're reading an array or if this is a label
				if ( close != "]" )
				{
					if ( t != "{" && t != "[" )
					{
						// t is a label
						if ( t[0] == '$' )
							next_parent = &macros.add_child( t ); // add macro
						else if ( !isgraph( t[0] ) )
							return zml_error( str, ec, "Invalid label " + t );
						else
							next_parent = &parent.add_child( t ); // add new item

						// read = or :
						t = get_zml_token( str, ec );
						if ( t == "=" || t == ": " )
							t = get_zml_token( str, ec );
						else if ( t != "{" && t != "[" )
							return zml_error( str, ec, "Expected '=', ':', '{' or '['" );
					}
					else
					{
						// the previous item has both a value and children
						if ( parent.size() == 0 )
							return zml_error( str, ec, "'{' or '[' has no matching '}' or ']'" );
						next_parent = &parent.back().second;
					}
				}
				else next_parent = &parent.add_child( "" ); // add array child

				// parse value element after
				if ( t == "{" ) // new group
					read_zml_layer( str, *next_parent, "}", ec, folder, included_files, root, macros );
				else if ( t == "[" ) // new array
					read_zml_layer( str, *next_parent, "]", ec, folder, included_files, root, macros );
				else if ( str_begins_with( t, '@' ) )
				{
					// assign previous value
					if ( auto ref_pn = root.try_get_query( t.substr( 1 ) ) )
						*next_parent = std::move( *ref_pn );
					else return zml_error( str, ec, "Could not find " + t );
				}
				else if ( t.size() >= 2 && t[0] == '$' && t[1] != '{' )
				{
					// replace non-inline macro
					auto it = std::find_if( macros.rbegin(), macros.rend(), [&]( auto& l ) { return t == l.first; } );
					if ( it != macros.rend() )
						*next_parent = it->second;
					else return zml_error( str, ec, "Undefined variable: " + t );
				}
				else
				{
					// replace inline macros
					for ( auto p = t.find( "${" ); p != string::npos; p = t.find( "${", p ) )
					{
						if ( auto pe = t.find( '}', p ); pe != string::npos )
						{
							auto it = std::find_if( macros.rbegin(), macros.rend(),
								[&]( auto& l ) { return t.compare( p + 2, pe - p - 2, l.first, 1 ) == 0; } );
							if ( it != macros.rend() )
							{
								t.replace( p, pe - p + 1, it->second.get_str() );
								p += it->second.get_str().size(); // advance p to avoid recursion
							}
							else return zml_error( str, ec, "Undefined variable: " + t.substr( p, pe - p + 1 ) );
						}
						else return zml_error( str, ec, "'${' without matching '}': " + t.substr( p ) );
					}

					// assign value
					next_parent->set_value( std::move( t ) );
				}
			}
		}

		// macros are limited by scope
		while ( macros.size() > macro_count )
			macros.pop_back();
	}

	prop_node parse_zml( char_stream& str, error_code* ec, const path& folder, vector<path>* included_files = nullptr )
	{
		str.set_operators( { "=", ": ", "{", "}", "[", "]", "#", "<<", ">>", "//", "/*", "*/" } );
		str.set_delimiter_chars( " \n\r\t\v" );
		str.set_quotation_chars( "\"'" );

		prop_node root, macros;
		read_zml_layer( str, root, "", ec, folder, included_files, root, macros );
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
		bool show_value = !pn.get_str().empty() || pn.size() == 0;
		xo_error_if( !show_label && show_value && !inside_array, "Value without label outside array" );
		if ( show_label )
			str << try_quoted( label, "{}[]#<>\\/" );
		if ( show_label && show_value )
			str << equals_str;
		if ( show_value )
			str << try_quoted( pn.get_str(), "{}[]#<>\\/" );
	}

	void write_zml_node_one_line( std::ostream& str, const string& label, const prop_node& pn, bool inside_array )
	{
		write_zml_kvp( str, label, pn, " = ", inside_array );
		if ( pn.size() > 0 )
		{
			// add space if there's a label
			if ( !label.empty() )
				str << ' ';

			// add children, either array or group
			bool is_array = pn.is_array();
			str << ( is_array ? "[ " : "{ " );
			for ( auto it = pn.begin(); it != pn.end(); ++it )
			{
				if ( it != pn.begin() ) str << " ";
				write_zml_node_one_line( str, it->first, it->second, is_array );
			}
			str << ( is_array ? " ]" : " }" );
		}
	}

	void write_zml_node( std::ostream& str, const string& label, const prop_node& pn, int level, bool inside_array )
	{
		if ( level > 0 )
			str << string( level - 1, '\t' );

		if ( pn.size() == 0 )
		{
			// just output kvp
			write_zml_kvp( str, label, pn, " = ", inside_array );
		}
		else
		{
			bool is_array = pn.is_array();

			// try oneliner
			const size_t max_oneliner_length = 80;
			if ( level != 0 && pn.count_layers() <= 2 && pn.count_children() <= 5 )
			{
				std::ostringstream oneliner;
				write_zml_node_one_line( oneliner, label, pn, is_array );
				if ( level + label.size() + 1 + oneliner.str().size() <= max_oneliner_length )
				{
					str << oneliner.str();
					return;
				}
			}

			// no oneliner, try normal output
			write_zml_kvp( str, label, pn, " = ", inside_array );
			if ( !label.empty() )
				str << ' '; // add space before { or [

			// normal multiline output
			if ( level != 0 )
				str << ( is_array ? "[" : "{" ) << std::endl;
			for ( auto it = pn.begin(); it != pn.end(); ++it )
			{
				write_zml_node( str, it->first, it->second, level + 1, is_array );
				str << std::endl;
			}
			if ( level != 0 )
				str << string( level - 1, '\t' ) << ( is_array ? "]" : "}" );
		}
	}

	void write_zml_node_concise( std::ostream& str, const string& label, const prop_node& pn, int level, bool inside_array )
	{
		write_zml_kvp( str, label, pn, "=", inside_array );
		if ( pn.size() > 0 )
		{
			// add children, either array or group
			bool is_array = pn.is_array();
			if ( level > 0 ) str << ( is_array ? '[' : '{' );
			for ( auto it = pn.begin(); it != pn.end(); ++it )
			{
				if ( it != pn.begin() ) str << " ";
				write_zml_node_concise( str, it->first, it->second, level + 1, is_array );
			}
			if ( level > 0 ) str << ( is_array ? ']' : '}' );
		}
	}

	std::istream& prop_node_serializer_zml::read_stream( std::istream& str )
	{
		xo_assert( read_pn_ );
		// #todo: more efficient. parser should be able to take any stream type.
		char_stream stream( string( std::istreambuf_iterator<char>( str ), {} ) );
		*read_pn_ = parse_zml( stream, ec_, file_folder_, &included_files_ );
		return str;
	}

	std::ostream& prop_node_serializer_zml::write_stream( std::ostream& str ) const
	{
		xo_assert( write_pn_ );
		if ( !write_pn_->empty() )
			write_zml_node( str, "", *write_pn_, 0, false );
		return str;
	}

	std::ostream& prop_node_serializer_zml_concise::write_stream( std::ostream& str ) const
	{
		xo_assert( write_pn_ );
		write_zml_node_concise( str, "", *write_pn_, 0, false );
		return str;
	}

	XO_API prop_node load_zml( const path& filename, error_code* ec, path parent_folder, vector<path>* included_files )
	{
		char_stream stream( load_string( filename ) );
		return parse_zml( stream, ec, filename.parent_path(), included_files );
	}

	XO_API void save_zml( const prop_node& pn, const path& filename, error_code* ec )
	{
		prop_node_serializer_zml().save_file( pn, filename, ec );
	}
}
