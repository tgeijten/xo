#include "prop_node.h"

#include "xo/container/container_tools.h"
#include "xo/numerical/math.h"
#include "xo/string/string_tools.h"

#include <iomanip>
#include <sstream>
#include "prop_node_tools.h"

namespace xo
{
	bool prop_node::has_value( const key_t& key ) const
	{
		if ( auto c = try_get_child( key ) )
			return c->has_value();
		else return false;
	}

	bool prop_node::has_key( const key_t& key ) const
	{
		return try_get_child( key ) != nullptr;
	}

	bool prop_node::has_any_key( std::initializer_list< key_t > keys ) const
	{
		for ( auto& k : keys ) if ( has_key( k ) ) return true; return false;
	}

	size_t prop_node::count_key( const key_t& key ) const
	{
		size_t n = 0;
		for ( auto& c : children )
			if ( c.first == key )
				++n;
		return n;
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

	bool prop_node::is_array() const
	{
		return find_if( *this, [&]( const prop_node::pair_t& n ) { return !n.first.empty(); } ) == end();
	}

	prop_node& prop_node::add_child()
	{
		return children.emplace_back( key_t(), prop_node() ).second;
	}

	prop_node& prop_node::add_child( const key_t& key )
	{
		return children.emplace_back( key, prop_node() ).second;
	}

	prop_node& prop_node::add_child( const key_t& key, const prop_node& pn )
	{
		return children.emplace_back( key, pn ).second;
	}

	prop_node& prop_node::add_child( const key_t& key, prop_node&& pn )
	{
		return children.emplace_back( key, std::move( pn ) ).second;
	}

	void prop_node::add_children( size_t count, const key_t& key, const value_t& value )
	{
		children.insert( children.end(), count, pair_t( key, value ) );
	}

	prop_node::iterator prop_node::insert( iterator pos, const_iterator first, const_iterator last )
	{
		return children.insert( pos, first, last );
	}

	prop_node::iterator prop_node::append( const prop_node& other )
	{
		return children.insert( children.end(), other.begin(), other.end() );
	}

	prop_node::iterator prop_node::append( prop_node&& other )
	{
		return children.insert( children.end(), std::make_move_iterator( other.begin() ), std::make_move_iterator( other.end() ) );
	}

	void prop_node::merge( const prop_node& other, bool overwrite )
	{
		if ( overwrite )
			value = other.value;
		for ( auto& o : other )
		{
			if ( auto c = try_get_child( o.first ) )
				c->merge( o.second, overwrite );
			else add_child( o.first, o.second );
		}
	}

	const prop_node& prop_node::get_child( const key_t& key ) const
	{
		for ( auto& c : children )
			if ( c.first == key )
				return c.second;
		xo_error( "Could not find key: " + key );
	}

	prop_node& prop_node::get_child( const key_t& key )
	{
		for ( auto& c : children )
			if ( c.first == key )
				return c.second;
		xo_error( "Could not find key: " + key );
	}

	const prop_node& prop_node::get_child( index_t idx ) const
	{
		xo_error_if( idx >= size(), "Invalid index: " + to_str( idx ) );
		return children[idx].second;
	}

	prop_node& prop_node::get_child( index_t idx )
	{
		xo_error_if( idx >= size(), "Invalid index: " + to_str( idx ) );
		return children[idx].second;
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

	prop_node& prop_node::get_or_add_child( const key_t& key )
	{
		if ( auto c = try_get_child( key ) )
			return *c;
		else return add_child( key );
	}

	const prop_node::key_t& prop_node::get_key( index_t idx ) const
	{
		xo_assert( idx < size() );
		access();
		return children[idx].first;
	}

	const prop_node* prop_node::try_get_query_key( const key_t& key ) const
	{
		if ( auto* c = try_get_child( key ) )
			return c;
		else if ( key.size() > 0 && key[0] == '#' )
		{
			auto idx = no_index;
			if ( from_str( key.substr( 1 ), idx ) && idx >= 1 && idx <= size() )
				return &children[idx - 1].second;
			else return nullptr;
		}
		else return nullptr;
	}

	const prop_node* prop_node::try_get_child( const key_t& key ) const
	{
		for ( auto& c : children )
			if ( c.first == key )
				return &c.second;
		return nullptr;
	}

	prop_node* prop_node::try_get_child( const key_t& key )
	{
		for ( auto& c : children )
			if ( c.first == key )
				return &c.second;
		return nullptr;
	}

	const prop_node* prop_node::try_find_child_recursively( const key_t& key ) const
	{
		for ( auto& c : children )
			if ( c.first == key )
				return &c.second;
			else if ( auto* pn = c.second.try_find_child_recursively( key ) )
				return pn;
		return nullptr;
	}

	const prop_node* prop_node::try_get_query( const key_t& key, const char delim ) const
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

	prop_node& prop_node::get_or_add_query( const key_t& key, const char delim )
	{
		auto p = key.find_first_of( delim );
		if ( p == string::npos )
		{
			if ( auto* c = try_get_query_key( key ) )
				return const_cast<prop_node&>( *c );
			else return add_child( key );
		}
		else
		{
			auto sub_key = key.substr( 0, p );
			if ( auto* c = try_get_query_key( sub_key ) )
				return const_cast<prop_node&>( *c ).get_or_add_query( mid_str( key, p + 1 ), delim );
			else return add_child( sub_key ).get_or_add_query( mid_str( key, p + 1 ), delim );
		}
	}

	pair<const prop_node*, index_t> prop_node::try_find_parent( const prop_node& pn ) const
	{
		for ( index_t idx = 0; idx < children.size(); ++idx )
			if ( &children[idx].second == &pn )
				return { this, idx };
		for ( const auto& c : children )
			if ( auto r = c.second.try_find_parent( pn ); r.first )
				return r;
		return { nullptr, no_index };
	}

	pair<prop_node*, index_t> prop_node::try_find_parent( const prop_node& pn )
	{
		auto r = const_cast<const prop_node*>( this )->try_find_parent( pn );
		return { const_cast<prop_node*>( r.first ), r.second };
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

	bool prop_node::is_accessed() const
	{
		return accessed_flag || !has_value();
	}

	size_t prop_node::count_unaccessed() const
	{
		size_t t = is_accessed() ? 0 : 1;
		for ( auto& c : children )
			t += c.second.count_unaccessed();
		return t;
	}

	void prop_node::set_accessed_recursively( bool access ) const
	{
		accessed_flag = access;
		for ( auto& c : children )
			c.second.set_accessed_recursively( access );
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

	std::ostream& to_stream( std::ostream& str, const prop_node& pn, size_t depth, size_t align )
	{
		if ( align == 0 )
			align = prop_node_align_width( pn, 2, depth );

		for ( auto& child : pn )
		{
			auto key = string( depth * 2, ' ' ) + child.first;
			str << key;
			if ( child.second.has_value() || child.second.size() == 0 )
				str << string( align - key.size(), ' ' ) << " = " << child.second.get_str();
			str << std::endl;

			to_stream( str, child.second, depth + 1, align );
		}

		return str;
	}

	string to_str( const prop_node& pn )
	{
		std::ostringstream str;
		to_stream( str, pn, 0, 0 );
		return str.str();
	}

	bool from_str( const string& s, prop_node& pn )
	{
		pn.set_value( string( s ) );
		return true;
	}
}
