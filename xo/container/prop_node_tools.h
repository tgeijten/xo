#pragma once

#include "xo/container/prop_node.h"
#include "xo/system/system_tools.h"
#include "xo/system/log.h"

#define INIT_PROP( _pn_, _var_, _default_ ) _var_ = _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), decltype( _var_ )( _default_ ) )
#define INIT_PROP_NAMED( _pn_, _var_, _name_, _default_ ) _var_ = _pn_.get< decltype( _var_ ) >( _name_, _default_ )
#define INIT_PROP_REQUIRED( _pn_, _var_ ) _var_ = _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ) )
#define INIT_PROP_NAMED_REQUIRED( _pn_, _var_, _name_ ) _var_ = _pn_.get< decltype( _var_ ) >( _name_ )

#define INIT_MEMBER( _pn_, _var_, _default_ ) _var_( _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), decltype( _var_ )( _default_ ) ) )
#define INIT_MEMBER_REQUIRED( _pn_, _var_ ) _var_( _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ) ) )

#define SET_PROP( _pn_, _var_ ) _pn_.set< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), _var_ )

namespace xo
{
	XO_API void log_unaccessed( const prop_node& pn, log::level level = log::warning_level, int depth = 0 );
	XO_API std::pair< bool, string > find_query_to_node( const prop_node* from, const prop_node* to, const char delim = '.' );
}
