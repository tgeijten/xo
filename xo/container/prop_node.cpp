#include "prop_node.h"
#include "xo/serialization/prop_node_serializer_zml.h"

#include <iomanip>

namespace xo
{
	prop_node::prop_node( const char* pn ) : accessed_flag( false )
	{
		*this = parse_zml( pn );
	}

	size_t prop_node::count_layers() const
	{
		size_t d = 0; for ( auto& c : children ) d = max( d, c.second.count_layers() + 1 ); return d;
	}

	size_t prop_node::count_children() const
	{
		size_t n = size(); for ( auto& c : children ) n += c.second.count_children(); return n;
	}

	void prop_node::merge( const prop_node& other, bool overwrite )
	{
		if ( overwrite )
			value = other.value;
		for ( auto& o : other )
		{
			auto it = find( o.first );
			if ( it == end() )
				push_back( o.first, o.second );
			else
				it->second.merge( o.second );
		}
	}

	const prop_node& prop_node::get_child_delimited( const key_t& key, const char delim ) const
	{
		auto p = key.find_first_of( delim );
		if ( p == string::npos )
			return get_child( key );
		else return get_child( key.substr( 0, p ) ).get_child_delimited( key.substr( p + 1 ), delim );
	}

	const xo::prop_node& prop_node::get_child( const key_t& key ) const
	{
		auto it = find( key );
		xo_error_if( it == end(), "Could not find key: " + key );
		access();
		return it->second;
	}

	xo::prop_node& prop_node::get_child( const key_t& key )
	{
		auto it = find( key );
		xo_error_if( it == end(), "Could not find key: " + key ); access();
		return it->second;
	}

	const xo::prop_node& prop_node::get_child( index_t idx ) const
	{
		xo_assert( idx < size() );
		access();
		return children[ idx ].second;
	}

	xo::prop_node& prop_node::get_child( index_t idx )
	{
		xo_assert( idx < size() );
		access();
		return children[ idx ].second;
	}

	xo::prop_node& prop_node::get_or_add_child( const key_t& key )
	{
		access(); auto it = find( key ); if ( it != end() ) return it->second.access(); else return push_back( key );
	}

	const xo::prop_node::key_t& prop_node::get_key( index_t idx ) const
	{
		xo_assert( idx < size() ); access();
		return children[ idx ].first;
	}

	const xo::prop_node* prop_node::try_get_child( const key_t& key ) const
	{
		access();
		auto it = find( key );
		return it != end() ? &( it->second.access() ) : nullptr;
	}

	xo::prop_node* prop_node::try_get_child( const key_t& key )
	{
		access();
		auto it = find( key );
		return it != end() ? &( it->second.access() ) : nullptr;
	}

	const xo::prop_node* prop_node::try_get_child_delimited( const key_t& key, const char delim ) const
	{
		auto p = key.find_first_of( delim );
		if ( p == string::npos ) return try_get_child( key );
		else if ( auto* c = try_get_child( key.substr( 0, p ) ) )
			return c->try_get_child_delimited( mid_str( key, p + 1 ), delim );
		else return nullptr;
	}

	xo::prop_node* prop_node::try_get_child_delimited( const key_t& key, const char delim )
	{
		auto p = key.find_first_of( delim );
		if ( p == string::npos ) return try_get_child( key );
		else if ( auto* c = try_get_child( key.substr( 0, p ) ) )
			return c->try_get_child_delimited( mid_str( key, p + 1 ), delim );
		else return nullptr;
	}

	const xo::prop_node* prop_node::try_get_child_delimited( std::initializer_list< key_t > keys, const char delim ) const
	{
		for ( auto& k : keys )
			if ( auto p = try_get_child_delimited( k, delim ) )
				return p;
		return nullptr;
	}

	int get_align_width( const prop_node& pn, int depth )
	{
		int width = 0;
		for ( auto& child : pn )
		{
			width = xo::max( width, depth * 2 + int( child.first.size() ) );
			width = xo::max( width, get_align_width( child.second, depth + 1 ) );
		}
		return width;
	}

	std::ostream& to_stream( std::ostream& str, const prop_node& pn, int depth, int align )
	{
		if ( align == 0 )
			align = get_align_width( pn, depth );

		for ( auto& child : pn )
		{
			string key_str( depth * 2, ' ' );
			key_str += child.first;

			str << std::left << std::setw( align ) << key_str;
			if ( child.second.has_value() || child.second.size() == 0 )
				str << std::setw( 0 ) << " = " << child.second.get_value();
			str << std::endl;

			to_stream( str, child.second, depth + 1, align );
		}

		return str;
	}

	bool prop_node::operator==( const prop_node& other )
	{
		if ( value != other.value )
			return false;

		if ( size() != other.size() )
			return false;

		for ( size_t i = 0; i < children.size(); ++i )
		{
			if ( children[i].first != other.children[i].first )
				return false;
			if ( children[i].second != other.children[i].second )
				return false;
		}

		return true;
	}
}
