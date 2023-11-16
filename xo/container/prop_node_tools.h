#pragma once

#include "xo/container/prop_node.h"
#include "xo/container/pair_type.h"
#include "xo/system/log_level.h"

#define INIT_PROP( _pn_, _var_, _default_ ) _var_ = _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), decltype( _var_ )( _default_ ) )
#define TRY_INIT_PROP( _pn_, _var_ ) if ( auto c = _pn_.try_get_child( ::xo::tidy_identifier( #_var_ ) ) ) _var_ = c->get< decltype( _var_ ) >();
#define TRY_GET_PROP( _pn_, _var_ ) _pn_.try_get( _var_, ::xo::tidy_identifier( #_var_ ) )
#define INIT_PROP_NAMED( _pn_, _var_, _name_, _default_ ) _var_ = _pn_.get< decltype( _var_ ) >( _name_, _default_ )
#define INIT_PROP_REQUIRED( _pn_, _var_ ) _var_ = _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ) )
#define INIT_PROP_NAMED_REQUIRED( _pn_, _var_, _name_ ) _var_ = _pn_.get< decltype( _var_ ) >( _name_ )

#define INIT_MEMBER( _pn_, _var_, _default_ ) _var_( _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), decltype( _var_ )( _default_ ) ) )
#define INIT_OPTIONAL_MEMBER( _pn_, _var_ ) _var_( _pn_.try_get< decltype( _var_ )::value_type >( ::xo::tidy_identifier( #_var_ ) ) )
#define INIT_MEMBER_REQUIRED( _pn_, _var_ ) _var_( _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ) ) )

#define SET_PROP( _pn_, _var_ ) _pn_.set< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), _var_ )

#define XO_DEFINE_FROM_PROP_NODE_FOR_TYPE( _type_ ) namespace xo { inline bool from_prop_node( const prop_node& pn, _type_& o ) { o = _type_( pn ); return true; } }

namespace xo
{
	XO_API size_t prop_node_align_width( const prop_node& pn, const size_t indent = 2, const size_t depth = 0 );
	XO_API void log_prop_node( const prop_node& pn, const size_t indent = 2, const size_t depth = 0, const size_t align = 0 );
	XO_API void log_unaccessed( const prop_node& pn, log::level level = log::level::warning, size_t depth = 0 );
	XO_API string to_str_unaccessed( const prop_node& pn );
	XO_API pair< bool, string > find_query_to_node( const prop_node* from, const prop_node* to, const char delim = '.' );
	XO_API prop_node prop_node_from_arg( int argc, const char* argv[] );
	XO_API string make_str_from_prop_node( const prop_node& pn );
	XO_API string make_prop_node_query( const prop_node& root, const prop_node& item );
}
