#include "prop_node.h"

#include "xo/serialization/prop_node_serializer_zml.h"
#include "xo/numerical/math.h"
#include "xo/string/string_tools.h"

#include <iomanip>

namespace xo
{
	prop_node::prop_node( const char* pn ) : accessed_flag( false )
	{
		*this = parse_zml( pn );
	}

	size_t prop_node::count_layers() const
	{
		size_t d = 0;
		for ( auto& c : children )
			d = max( d, c.second.count_layers() + 1 );
		return d;
	}

	size_t prop_node::count_children() const
	{
		size_t n = size();
		for ( auto& c : children )
			n += c.second.count_children();
		return n;
	}

	void prop_node::merge( const prop_node& other, bool overwrite )
	{
		if ( overwrite )
			value = other.value;
		for ( auto& o : other )
		{
			if ( auto c = try_get_child( o.first ) )
				c->merge( o.second, overwrite );
			else push_back( o.first, o.second );
		}
	}

	const xo::prop_node& prop_node::get_child( const key_t& key ) const
	{
		for ( auto& c : children )
			if ( c.first == key )
				return c.second;
		xo_error( "Could not find key: " + key );
	}

	xo::prop_node& prop_node::get_child( const key_t& key )
	{
		for ( auto& c : children )
			if ( c.first == key )
				return c.second;
		xo_error( "Could not find key: " + key );
	}

	const xo::prop_node& prop_node::get_child( index_t idx ) const
	{
		xo_error_if( idx >= size(), "Invalid index: " + to_str( idx ) );
		return children[ idx ].second;
	}

	xo::prop_node& prop_node::get_child( index_t idx )
	{
		xo_error_if( idx >= size(), "Invalid index: " + to_str( idx ) );
		return children[ idx ].second;
	}

	const prop_node& prop_node::get_any_child( std::initializer_list< key_t > keys ) const
	{
		for ( auto& key : keys )
			if ( auto c = try_get_child( key ) )
				return *c;
		xo_error( "Could not find any key: " + concatenate_str( keys, " or " ) );
	}

	const prop_node* prop_node::try_get_any_child( std::initializer_list< key_t > keys ) const
	{
		for ( auto& key : keys )
			if ( auto c = try_get_child( key ) )
				return c;
		return nullptr;
	}

	xo::prop_node& prop_node::get_or_add_child( const key_t& key )
	{
		if ( auto c = try_get_child( key ) )
			return *c;
		else return push_back( key );
	}

	const xo::prop_node::key_t& prop_node::get_key( index_t idx ) const
	{
		xo_assert( idx < size() );
		access();
		return children[ idx ].first;
	}

	const xo::prop_node* prop_node::try_get_query_key( const key_t& key ) const
	{
		if ( auto* c = try_get_child( key ) )
			return c;
		else if ( key.size() > 0 && key[ 0 ] == '#' )
		{
			auto idx = from_str( key.substr( 1 ), 0 );
			if ( idx > 0 && idx <= size() )
				return &children[ idx - 1 ].second;
			else return nullptr;
		}
		else return nullptr;
	}

	const xo::prop_node* prop_node::try_get_query( const key_t& key, const char delim ) const
	{
		auto p = key.find_first_of( delim );
		if ( p == string::npos )
			return try_get_query_key( key );
		else if ( auto* c = try_get_query_key( key.substr( 0, p ) ) )
			return c->try_get_query( mid_str( key, p + 1 ), delim );
		else return nullptr;
	}

	prop_node* prop_node::try_get_query( const key_t& key, const char delim )
	{
		return const_cast<prop_node*>( const_cast<const prop_node*>( this )->try_get_query( key, delim ) );
	}

	xo::prop_node& prop_node::get_or_add_query( const key_t& key, const char delim )
	{
		auto p = key.find_first_of( delim );
		if ( p == string::npos )
		{
			if ( auto* c = try_get_query_key( key ) )
				return const_cast<prop_node&>( *c );
			else return push_back( key );
		}
		else
		{
			auto sub_key = key.substr( 0, p );
			if ( auto* c = try_get_query_key( sub_key ) )
				return const_cast<prop_node&>( *c ).get_or_add_query( mid_str( key, p + 1 ), delim );
			else return push_back( sub_key ).get_or_add_query( mid_str( key, p + 1 ), delim );
		}
	}

	bool prop_node::erase( const key_t& key )
	{
		for ( auto it = begin(); it != end(); ++it )
			if ( it->first == key )
				return erase( it ), true;
		return false;
	}

	bool prop_node::erase_query( const key_t& query, const char delim )
	{
		auto p = query.find_first_of( delim );
		if ( p == string::npos )
			return erase( query );
		else if ( auto* c = try_get_query( query.substr( 0, p ) ) )
			return c->erase_query( mid_str( query, p + 1 ), delim );
		else return false;
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

	bool prop_node::operator==( const prop_node& other ) const
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
