#include "serialize.h"

#include "prop_node_serializer_xml.h"
#include "prop_node_serializer_ini.h"
#include "prop_node_serializer_zml.h"
#include <fstream>
#include "xo/container/prop_node.h"

namespace xo
{
	factory< prop_node_serializer >& get_serializer_factory()
	{
		static factory<prop_node_serializer> f = factory<prop_node_serializer>()
			.register_type< prop_node_serializer_xml >( "xml" )
			.register_type< prop_node_serializer_ini >( "ini" )
			.register_type< prop_node_serializer_zml >( "zml" );

		return f;
	}

	u_ptr< prop_node_serializer > make_serializer( const string& file_type, prop_node& pn, error_code* ec, path p )
	{
		if ( auto s = get_serializer_factory().try_create( file_type ) )
		{
			s->write_pn_ = &pn;
			s->read_pn_ = &pn;
			s->ec_ = ec;
			s->file_folder_ = p;
			return s;
		}
		else return set_error_or_throw( ec, "Unsupported file format: " + file_type ), nullptr;
	}

	u_ptr< prop_node_serializer > make_serializer( const string& file_type, const prop_node& pn, error_code* ec, path p, vector<path>* included_files )
	{
		if ( auto s = get_serializer_factory().try_create( file_type ) )
		{
			s->write_pn_ = &pn;
			s->ec_ = ec;
			s->file_folder_ = p;
			s->included_files_ = included_files;
			return s;
		}
		else return set_error_or_throw( ec, "Unsupported file format: " + file_type ), nullptr;
	}

	prop_node load_file( const path& filename, error_code* ec, vector<path>* included_files )
	{
		prop_node pn;
		if ( auto s = make_serializer( filename.extension_no_dot().str(), pn, ec, path(), included_files ) )
			return s->load_file( filename, ec );
		else return set_error_or_throw( ec, "Unsupported file format for " + filename.str() ), prop_node();
	}

	prop_node load_file( const path& filename, const string& file_type, error_code* ec, vector<path>* included_files )
	{
		prop_node pn;
		if ( auto s = make_serializer( file_type, pn, ec, path(), included_files ) )
			return s->load_file( filename, ec );
		else return set_error_or_throw( ec, "Unknown file format: " + file_type ), prop_node();
	}

	bool save_file( const prop_node& pn, const path& filename, error_code* ec )
	{
		if ( auto s = make_serializer( filename.extension_no_dot().str(), pn, ec ) )
			s->save_file( pn, filename, ec );
		else set_error_or_throw( ec, "Could not deduce file format for " + filename.str() );
		return ec ? ec->good() : true;
	}

	bool save_file( const prop_node& pn, const path& filename, const string& file_type, error_code* ec )
	{
		if ( auto s = make_serializer( file_type, pn, ec ) )
			s->save_file( pn, filename, ec );
		else set_error_or_throw( ec, "Unknown file format: " + file_type );
		return ec ? ec->good() : true;
	}

	prop_node load_file_with_include( const path& filename, const string& include_directive, int level, vector<path>* included_files );

	void resolve_include_files( prop_node& pn, const path& filename, const string& include_directive, int level, vector<path>* included_files )
	{
		for ( auto iter = pn.begin(); iter != pn.end(); )
		{
			if ( iter->first == include_directive )
			{
				// load included file using filename path
				path include_path = filename.parent_path() / iter->second.get< path >( "file" );
				bool merge_children = iter->second.get< bool >( "merge_children", false );
				auto included_props = load_file_with_include( include_path, include_directive, level + 1, included_files );
				xo_error_if( included_props.size() != 1, "Included files must have a single root node" );
				auto& included_children = included_props.begin()->second;

				if ( included_files )
					included_files->emplace_back( include_path );

				// remove the include node
				iter = pn.erase( iter );

				// merge or include, depending on options
				if ( merge_children )
				{
					pn.merge( included_children, false );
					iter = pn.begin(); // reset the iterator, which has become invalid after merge
				}
				else
				{
					// insert the children at the INCLUDE spot
					iter = pn.insert( iter, included_props.begin(), included_props.end() );
				}
			}
			else
			{
				// search in children
				resolve_include_files( iter->second, filename, include_directive, level, included_files );
				++iter;
			}
		}
	}

	prop_node load_file_with_include( const path& filename, const string& include_directive, int level, vector<path>* included_files )
	{
		xo_error_if( level >= 100, "Exceeded maximum include level, check for loops in includes" );

		prop_node pn = load_file( filename, nullptr, included_files  );
		resolve_include_files( pn, filename, include_directive, level, included_files );
		pn.set_accessed_recursively( false );

		return pn;
	}

	prop_node load_file_with_include( const path& filename, const string& include_directive, vector<path>* included_files )
	{
		return load_file_with_include( filename, include_directive, 0, included_files );
	}
}
