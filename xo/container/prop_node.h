#pragma once

#include "xo/xo_types.h"
#include "xo/system/assert.h"
#include "xo/system/system_tools.h"
#include "xo/string/string_type.h"
#include "xo/string/string_cast.h"
#include "xo/string/string_tools.h"
#include "xo/container/vector_type.h"
#include "xo/container/pair_type.h"
#include "xo/utility/optional.h"
#include "xo/utility/type_traits.h"
#include "xo/container/view_if.h"

#include <initializer_list>

namespace xo
{
	/// prop_node class
	class XO_API prop_node
	{
	public:
		using key_t = string;
		using value_t = string;
		using pair_t = pair< key_t, prop_node >;
		using container_t = vector< pair_t >;
		using iterator = container_t::iterator;
		using const_iterator = container_t::const_iterator;
		using reverse_iterator = container_t::reverse_iterator;
		using const_reverse_iterator = container_t::const_reverse_iterator;

		/// constructors
		prop_node() : accessed_flag( false ), value() {}
		prop_node( const prop_node& other ) = default;
		prop_node( prop_node&& other ) = default;
		explicit prop_node( const value_t& v ) : accessed_flag( false ), value( v ) {}
		explicit prop_node( value_t&& v ) : accessed_flag( false ), value( std::move( v ) ) {}
		explicit prop_node( const char* v ) : accessed_flag( false ), value( v ) {}
		explicit prop_node( std::initializer_list< pair_t > c ) : accessed_flag( false ), children( c ) {}

		/// destructor (non-virtual)
		~prop_node() = default;

		/// copy assignment
		prop_node& operator=( const prop_node& other ) = default;
		prop_node& operator=( prop_node&& other ) = default;
		prop_node& set( prop_node&& pn ) { return operator=( std::move( pn ) ); }
		prop_node& set( const prop_node& pn ) { return operator=( pn ); }

		/// assignment
		template< typename T > prop_node& operator=( const T& v ) { return set( v ); }
		template< typename T > prop_node& set( const T& v );

		/// set value of child node
		template< typename T > prop_node& set( const key_t& key, const T& v );

		/// set the value of this node directly (does not affect children)
		template< typename T > prop_node& set_value( const T& v ) { value = to_str( v ); return *this; }
		prop_node& set_value( value_t&& v ) { value = std::move( v ); return *this; }

		/// get the value of this node, throws if conversion fails
		template< typename T > T get() const;

		/// get the value of a child node, throws if value does not exist
		template< typename T > T get( const key_t& key ) const;

		/// get the value of a child node, throws if value does not exist
		template< typename T > T get( index_t idx ) const;

		/// get the value of a child node, or a default value if it doesn't exist
		template< typename T > T get( const key_t& key, const T& def ) const;

		/// get an optional value of this node
		template< typename T > optional<T> try_get() const;

		/// get an optional value of a child node
		template< typename T > optional<T> try_get( const key_t& key ) const;

		/// get a value of a child node, only stores if exists
		template< typename T > bool try_get( T& value, const key_t& key ) const;

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > T get_any( std::initializer_list< key_t > keys, const T& def ) const;

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > T get_any( std::initializer_list< key_t > keys ) const;

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > optional< T > try_get_any( std::initializer_list< key_t > keys ) const;

		/// equality operators
		bool operator==( const prop_node& other ) const;
		bool operator!=( const prop_node& other ) const { return !( *this == other ); }

		/// see if this prop_node has a value
		bool has_value() const { return !value.empty(); }
		bool has_value( const key_t& key ) const;

		/// see if this prop_node has a specific key
		bool has_key( const key_t& key ) const;

		/// see if this prop_node has any specific key
		bool has_any_key( std::initializer_list< key_t > keys ) const;

		/// number of direct child keys
		size_t size() const { return children.size(); }

		/// number of child layers
		size_t count_layers() const;

		/// number of children (recursively)
		size_t count_children() const;

		/// returns true if all keys are empty
		bool is_array() const;

		/// true if prop_node has a value or a key
		bool empty() const { return value.empty() && children.empty(); }

		/// clear value and keys
		void clear() { value.clear(); children.clear(); }

		/// get raw value_t reference
		const value_t& raw_value() const { access(); return value; }

		/// set the value of a child node, accessing children through delimiter character
		template< typename T > prop_node& set_query( const key_t& query, const T& v, const char delim = '.' );

		/// add a child node with a value
		template< typename T > prop_node& add_key_value( const key_t& key, const T& value );
		template< typename T > void add_value( const T& value ) { children.emplace_back( key_t(), to_prop_node( value ) ); }

		/// add a child node
		prop_node& add_child();
		prop_node& add_child( const key_t& key );
		prop_node& add_child( const key_t& key, const prop_node& pn );
		prop_node& add_child( const key_t& key, prop_node&& pn );

		/// insert children
		iterator insert( iterator pos, const_iterator first, const_iterator last );

		// append children from another prop_node
		iterator append( const prop_node& other );
		iterator append( prop_node&& other );
		void merge( const prop_node& other, bool overwrite = false );

		/// reserve children
		void reserve( size_t n ) { children.reserve( n ); }

		/// access child by name, throws if not existing
		const prop_node& get_child( const key_t& key ) const;
		const prop_node& get_any_child( std::initializer_list< key_t > keys ) const;
		const prop_node* try_get_any_child( std::initializer_list< key_t > keys ) const;

		const prop_node& operator[]( const key_t& key ) const { return get_child( key ); }
		prop_node& get_child( const key_t& key );

		/// get a child node or add it if not existing
		prop_node& get_or_add_child( const key_t& key );
		prop_node& operator[]( const key_t& key ) { return get_or_add_child( key ); }

		/// get a child node by index, throws if invalid
		const prop_node& get_child( index_t idx ) const;
		prop_node& get_child( index_t idx );
		const prop_node& operator[]( index_t idx ) const { return get_child( idx ); }
		prop_node& operator[]( index_t idx ) { return get_child( idx ); }

		/// get a child node, return nullptr if not existing
		const prop_node* try_get_child( const key_t& key ) const;
		prop_node* try_get_child( const key_t& key );

		/// get a child node using delimiters, return nullptr if not existing
		const prop_node* try_get_query( const key_t& query, const char delim = '.' ) const;
		prop_node* try_get_query( const key_t& query, const char delim = '.' );
		prop_node& get_or_add_query( const key_t& query, const char delim = '.' );

		/// get key by index
		const key_t& get_key( index_t idx ) const;

		/// begin of child nodes
		iterator begin() { access(); return children.begin(); }
		const_iterator begin() const { access(); return children.begin(); }
		const_iterator cbegin() const { access(); return children.cbegin(); }
		reverse_iterator rbegin() { access(); return children.rbegin(); }
		const_reverse_iterator rbegin() const { access(); return children.rbegin(); }
		const pair_t& front() const { return children.front(); }
		pair_t& front() { return children.front(); }

		/// end of child nodes
		iterator end() { return children.end(); }
		const_iterator end() const { return children.end(); }
		const_iterator cend() const { return children.cend(); }
		reverse_iterator rend() { access(); return children.rend(); }
		const_reverse_iterator rend() const { access(); return children.rend(); }
		const pair_t& back() const { return children.back(); }
		pair_t& back() { return children.back(); }

		/// access selection with specific key
		auto select( const string& key ) const
		{ return make_view_if( begin(), end(), [=]( const pair_t& kvp ) { return kvp.first == key; } ); }

		/// access items with specific pattern
		auto select_pattern( const string& pattern ) const
		{ return make_view_if( begin(), end(), [=]( const pair_t& kvp ) { return pattern_match( kvp.first, pattern ); } ); }

		/// erase a child
		iterator erase( const_iterator it ) { return children.erase( it ); }
		bool erase( const key_t& key );
		bool erase_query( const key_t& query, const char delim = '.' );

		void pop_back() { children.pop_back(); }

		/// see if this node has been accessed
		bool is_accessed() const;
		size_t count_unaccessed() const;
		void set_accessed_recursively( bool access ) const;
		void access() const { accessed_flag = true; }

	private:
		const prop_node* try_get_query_key( const key_t& key ) const;

		mutable bool accessed_flag;
		value_t value;
		container_t children;
	};

	/// prop_node literal, 'using namespace xo::literals' puts them outside the xo namespace
	inline namespace literals {
		inline prop_node operator"" _pn( const char* v, size_t s ) { return prop_node( v ); }
	}

	/// convert prop_node to string
	XO_API string to_str( const prop_node& pn );

	/// convert string to prop_node: this simply sets the value!
	XO_API bool from_str( const string& s, prop_node& pn );

	/// default conversion from T to prop_node, uses to_str()
	template< typename T > prop_node to_prop_node( const T& v ) {
		return prop_node( to_str( v ) );
	}

	/// default conversion from prop_node to T, uses to_str()
	template< typename T > bool from_prop_node( const prop_node& pn, T& v ) {
		return from_str( pn.raw_value(), v ); 
	};

	template< typename T > prop_node to_prop_node( const vector<T>& vec ) {
		prop_node pn;
		for ( size_t i = 0; i < vec.size(); ++i )
			pn.add_child( "", to_prop_node( vec[ i ] ) );
		return pn;
	}

	template< typename T > bool from_prop_node( const prop_node& pn , vector<T>& vec ) {
		vec.resize( pn.size(), T() );
		for ( index_t i = 0; i < pn.size(); ++i )
			if ( !from_prop_node( pn[ i ], vec[ i ] ) )
				return false;
		pn.access();
		return true;
	};

	//
	// prop_node class implementations
	//

	template< typename T >
	prop_node& prop_node::set( const T& v ) {
		return *this = to_prop_node( v );
	}

	template< typename T >
	prop_node& prop_node::set( const key_t& key, const T& v ) {
		return get_or_add_child( key ).set( v );
	}

	template< typename T >
	T prop_node::get() const {
		typename remove_const<T>::type value;
		if ( from_prop_node( *this, value ) )
			return value;
		else xo_error( "Could not convert \"" + raw_value() + "\" to " + get_type_name<T>() );
	}

	template< typename T >
	T prop_node::get( const key_t& key ) const {
		return get_child( key ).get< T >();
	}

	template< typename T >
	T prop_node::get( index_t idx ) const {
		return get_child( idx ).get< T >();
	}

	template< typename T >
	T prop_node::get( const key_t& key, const T& def ) const {
		if ( auto c = try_get_child( key ) )
			return c->get<T>();
		else return def;
	}

	template< typename T >
	optional<T> prop_node::try_get() const {
		T value;
		if ( from_prop_node( *this, value ) )
			return value;
		else return optional<T>();
	}

	template< typename T >
	optional<T> prop_node::try_get( const key_t& key ) const {
		if ( auto c = try_get_child( key ) )
			return c->try_get< T >();
		else return optional< T >();
	}

	template< typename T >
	bool prop_node::try_get( T& value, const key_t& key ) const	{
		if ( auto c = try_get_child( key ) ) {
			value = c->get< T >(); return true;
		}
		else return false;
	}

	template< typename T >
	T prop_node::get_any( std::initializer_list< key_t > keys, const T& def ) const	{
		if ( auto c = try_get_any_child( keys ) )
			return c->get< T >();
		return def;
	}

	template< typename T >
	T prop_node::get_any( std::initializer_list< key_t > keys ) const {
		if ( auto c = try_get_any_child( keys ) )
			return c->get< T >();
		xo_error( "Could not find key: " + concatenate_str( keys, " or " ) );
	}

	template< typename T > optional< T >
	prop_node::try_get_any( std::initializer_list< key_t > keys ) const {
		if ( auto c = try_get_any_child( keys ) )
			return c->get< T >();
		return optional< T >();
	}

	template< typename T >
	prop_node& prop_node::set_query( const key_t& query, const T& v, const char delim ) {
		return get_or_add_query( query, delim ).set( v );
	}

	template< typename T >
	prop_node& prop_node::add_key_value( const key_t& key, const T& value ) {
		children.emplace_back( key, to_prop_node( value ) );
		return children.back().second;
	}
}
