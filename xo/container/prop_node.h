#pragma once

#include "xo/system/platform.h"
#include "xo/system/assert.h"
#include "xo/string/string_tools.h"
#include "xo/string/string_cast.h"
#include "xo/utility/optional.h"
#include "xo/container/view_if.h"
#include "xo/container/container_tools.h"

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	class prop_node;

	/// make a prop_node with a value
	template< typename T > prop_node make_prop_node( const T& value );

	/// forward declare prop_node_cast
	template< typename T, typename E = void > struct prop_node_cast;

	/// prop_node class
	class XO_API prop_node
	{
	public:
		typedef string key_t;
		typedef string value_t;
		typedef std::pair< key_t, prop_node > pair_t;
		typedef std::vector< pair_t > container_t;
		typedef container_t::iterator iterator;
		typedef container_t::const_iterator const_iterator;

		/// constructors
		prop_node() : accessed_flag( false ) {}
		prop_node( const prop_node& other ) = default;
		prop_node( prop_node&& other ) = default;
		prop_node( const char* pn );
		prop_node( const value_t& v ) : accessed_flag( false ), value( v ) {}
		prop_node( value_t&& v ) : accessed_flag( false ), value( std::move( v ) ) {}

		/// destructor (non-virtual)
		~prop_node() {}

		/// assignment operators
		prop_node& operator=( const prop_node& other ) = default;
		prop_node& operator=( prop_node&& other ) = default;
		template< typename T > prop_node& operator=( const T& v ) { *this = prop_node_cast<T>::to( v ); return *this; }

		/// equality operators
		bool operator==( const prop_node& other );
		bool operator!=( const prop_node& other ) { return !( *this == other ); }

		/// get the value of this node
		template< typename T > T get() const { access(); return prop_node_cast<T>::from( *this ); }

		/// get the value of a child node
		template< typename T > T get( const key_t& key ) const { return get_child( key ).get< T >(); }

		/// get the value of a child node
		template< typename T > T get( index_t idx ) const { return get_child( idx ).get< T >(); }

		/// get the value of a child node, or a default value if it doesn't exist
		template< typename T > T get( const key_t& key, const T& def ) const
		{ auto it = find( key ); return ( it != end() ) ? it->second.get< T >() : def; }

		/// get an optional value of a child node
		template< typename T > optional< T > try_get( const key_t& key ) const
		{ auto it = find( key ); return ( it != end() ) ? it->second.get< T >() : optional< T >(); }

		/// get a value of a child node, only stores if exists
		template< typename T > bool try_get( T& value, const key_t& key ) const
		{ auto it = find( key ); if ( it != end() ) { value = it->second.get< T >(); return true; } else return false; }

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > T get_any( std::initializer_list< key_t > keys, const T& def ) const
		{ for ( auto& key : keys ) { const auto it = find( key ); if ( it != end() ) return it->second.get< T >(); } return def; }

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > T get_any( std::initializer_list< key_t > keys ) const
		{ for ( auto& key : keys ) { const auto it = find( key ); if ( it != end() ) return it->second.get< T >(); } xo_error( "Could not find key: " + to_str( keys ) ); }

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > optional< T > try_get_any( std::initializer_list< key_t > keys ) const
		{ for ( auto& key : keys ) { const auto it = find( key ); if ( it != end() ) return it->second.get< T >(); } return optional< T >(); }

		/// get the value of a child node, accessing children through delimiter character
		template< typename T > T get_delimited( const key_t& key, const char delim = '.' ) const {
			auto p = key.find_first_of( delim );
			if ( p == string::npos ) return get< T >( key );
			else return get_child( key.substr( 0, p ) ).get_delimited< T >( mid_str( key, p + 1 ), delim );
		}

		/// get the value of a child node, accessing children through delimiter character
		template< typename T > optional< T > try_get_delimited( const key_t& key, const char delim = '.' ) const {
			if ( auto* pn = try_get_child_delimited( key ) )
				return pn->get< T >();
			else return optional< T >();
		}

		/// see if this prop_node has a value
		bool has_value() const { return !value.empty(); }
		bool has_value( const key_t& key ) const { auto c = find( key ); return c != end() && c->second.has_value(); }

		/// see if this prop_node has a specific key
		bool has_key( const key_t& key ) const { return find( key ) != end(); }

		/// see if this prop_node has any specific key
		bool has_any_key( std::initializer_list< key_t > keys ) const { for ( auto& k : keys ) if ( has_key( k ) ) return true; return false; }

		/// number of direct child keys
		size_t size() const { return children.size(); }

		/// number of child layers
		size_t count_layers() const;

		/// number of children (recursively)
		size_t count_children() const;

		/// true if prop_node has a value or a key
		bool empty() const { return value.empty() && children.empty(); }

		/// clear value and keys
		void clear() { value.clear(); children.clear(); }

		/// access value_t value
		const value_t& get_value() const { access(); return value; }

		/// set the value of this node
		prop_node& set( prop_node&& pn ) { *this = std::move( pn ); return *this; }
		prop_node& set( const prop_node& pn ) { *this = pn; return *this; }
		template< typename T > prop_node& set( const T& v ) { *this = prop_node_cast<T>::to( v ); return *this; }

		/// set the value of this node
		template< typename T > prop_node& set_value( const T& v ) { value = to_str( v ); return *this; }
		prop_node& set_value( value_t&& v ) { value = std::move( v ); return *this; }

		/// set the value of a child node, the node is created if not existing
		template< typename T > prop_node& set( const key_t& key, const T& v )
		{ auto it = find( key ); if ( it == end() ) return push_back( key, v ); else return it->second.set( v ); }

		/// set the value of a child node, accessing children through delimiter character
		template< typename T > prop_node& set_delimited( const key_t& key, const T& v, const char delim = '.' ) {
			auto p = key.find_first_of( delim );
			if ( p == string::npos ) return set( key, v );
			else return get_or_add_child( key.substr( 0, p ) ).set_delimited( mid_str( key, p + 1 ), v, delim );
		}

		/// add a node with a value
		template< typename T > prop_node& push_back( const key_t& key, const T& value ) { children.emplace_back( key, make_prop_node( value ) ); return children.back().second; }
		prop_node& push_back( const key_t& key, const prop_node& pn ) { children.emplace_back( key, pn ); return children.back().second; }
		prop_node& push_back( const key_t& key, prop_node&& pn ) { children.emplace_back( key, std::move( pn ) ); return children.back().second; }
		prop_node& push_back( const key_t& key ) { children.emplace_back( key, prop_node() ); return children.back().second; }

		/// insert children
		iterator insert( iterator pos, const_iterator first, const_iterator last ) { return children.insert( pos, first, last ); }

		// append children from another prop_node
		iterator append( const prop_node& other ) { return children.insert( children.end(), other.begin(), other.end() ); }
		iterator append( prop_node&& other ) { return children.insert( children.end(), std::make_move_iterator( other.begin() ), std::make_move_iterator( other.end() ) ); }
		void merge( const prop_node& other, bool overwrite = false );

		/// reserve children
		void reserve( size_t n ) { children.reserve( n ); }

		/// access child by name, throws if not existing
		const prop_node& get_child( const key_t& key ) const;
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

		/// get a child node using delimiters, throws exception if not existing
		const prop_node& get_child_delimited( const key_t& key, const char delim = '.' ) const;

		/// get a child node using delimiters, return nullptr if not existing
		const prop_node* try_get_child_delimited( const key_t& key, const char delim = '.' ) const;
		prop_node* try_get_child_delimited( const key_t& key, const char delim = '.' );
		const prop_node* try_get_child_delimited( std::initializer_list< key_t > keys, const char delim = '.' ) const;

		/// get key by index
		const key_t& get_key( index_t idx ) const;

		/// find a child node
		iterator find( const key_t& key ) { return find_if( children, [&]( const pair_t& e ) { return e.first == key; } ); }
		const_iterator find( const key_t& key ) const { return find_if( children, [&]( const pair_t& e ) { return e.first == key; } ); }

		/// begin of child nodes
		iterator begin() { access(); return children.begin(); }
		const_iterator begin() const { access(); return children.begin(); }
		const_iterator cbegin() const { access(); return children.cbegin(); }
		const pair_t& front() const { return children.front(); }
		pair_t& front() { return children.front(); }

		/// end of child nodes
		iterator end() { return children.end(); }
		const_iterator end() const { return children.end(); }
		const_iterator cend() const { return children.cend(); }
		const pair_t& back() const { return children.back(); }
		pair_t& back() { return children.back(); }

		/// access selection of a specific type
		auto select( const string& key ) const { return make_view_if( begin(), end(), [=]( const pair_t& kvp ) { return kvp.first == key; } ); }
		auto select_pattern( const string& pattern ) const { return make_view_if( begin(), end(), [=]( const pair_t& kvp ) { return pattern_match( kvp.first, pattern ); } ); }

		/// erase a child
		iterator erase( const_iterator it ) { return children.erase( it ); }
		void pop_back() { children.pop_back(); }

		/// see if this node has been accessed
		bool is_accessed() const { return accessed_flag; }
		size_t count_unaccessed() const { size_t t = is_accessed() ? 0 : 1; for ( auto& c : children ) t += c.second.count_unaccessed(); return t; }
		void clear_accessed_recursively() { accessed_flag = false; for ( auto& c : children ) c.second.clear_accessed_recursively(); }

	private:
		const prop_node& access() const { accessed_flag = true; return *this; }
		prop_node& access() { accessed_flag = true; return *this; }

		mutable bool accessed_flag;
		value_t value;
		container_t children;
	};

	/// prop_node_cast and specializations
	template<> struct prop_node_cast<prop_node> {
		static prop_node from( const prop_node& pn ) { return pn; }
		static prop_node to( const prop_node& value ) { return value; }
	};

	template< typename T > struct prop_node_cast<std::vector<T>> {
		static std::vector<T> from( const prop_node& pn ) {
			std::vector<T> vec; for ( auto& p : pn ) vec.push_back( p.second.get<T>() ); return vec;
		}
		static prop_node to( const std::vector<T>& vec ) {
			prop_node pn; for ( size_t i = 0; i < vec.size(); ++i ) pn.push_back( stringf( "item%d", i ), make_prop_node( vec[ i ] ) ); return pn;
		}
	};

	template< typename T > struct is_prop_node_constructable { static const bool value = false; };
	template< typename T > struct prop_node_cast<T, typename std::enable_if< is_prop_node_constructable< T >::value >::type> {
		static T from( const prop_node& pn ) { return T( pn ); }
		static prop_node to( const T& value ) { xo_error( "Cannot convert this class to prop_node" ); }
	};

	template< std::size_t N > struct prop_node_cast<char[ N ]> {
		static const char* from( const prop_node& pn ) { xo_error( "Cannot convert prop_node to string literal" ); }
		static prop_node to( const char* value ) { return prop_node( string( value ) ); }
	};

	template< typename T, typename E > struct prop_node_cast {
		static T from( const prop_node& pn ) { return string_cast<T>::from( pn.get_value() ); }
		static prop_node to( const T& value ) { return prop_node( to_str( value ) ); }
	};

	template< typename T > prop_node make_prop_node( const T& value ) { return prop_node_cast<T>::to( value ); }

	/// stream operator
	XO_API std::ostream& to_stream( std::ostream& str, const prop_node& pn, int depth = 0, int key_align = 0 );
	inline std::ostream& operator<<( std::ostream& str, const prop_node& pn ) { return to_stream( str, pn ); }
}

#define IS_PROP_NODE_CONSTRUCTABLE( _class_name_ ) \
template<> struct ::xo::is_prop_node_constructable< _class_name_ > { static const bool value = true; };

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
