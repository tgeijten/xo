#pragma once

#include "xo/container/prop_node.h"
#include "xo/filesystem/path.h"
#include "xo/system/error_code.h"
#include "xo/system/system_tools.h"
#include "xo/system/log.h"

#define INIT_PROP( _pn_, _var_, _default_ ) _var_ = _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), decltype( _var_ )( _default_ ) )
#define INIT_PROP_NAMED( _pn_, _var_, _name_, _default_ ) _var_ = _pn_.get< decltype( _var_ ) >( _name_, _default_ )
#define INIT_PROP_REQUIRED( _pn_, _var_ ) _var_ = _pn_.get< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ) )
#define INIT_PROP_NAMED_REQUIRED( _pn_, _var_, _name_ ) _var_ = _pn_.get< decltype( _var_ ) >( _name_ )

#define SET_PROP( _pn_, _var_ ) _pn_.set< decltype( _var_ ) >( ::xo::tidy_identifier( #_var_ ), _var_ )
#define SET_PROP_NAMED( _pn_, _var_, _name_ ) _pn_.set< decltype( _var_ ) >( _name_, _var_ )

namespace xo
{
	/// merge prop_nodes
	XO_API void merge_prop_nodes( prop_node& pn, const prop_node& other, bool overwrite );

	/// insert prop_nodes
	XO_API prop_node load_file_with_include( const path& filename, const string& include_directive = "INCLUDE", int level = 0 );

	XO_API void log_unaccessed( const prop_node& pn, log::level level = log::warning_level, int depth = 0 );
}
