#pragma once

#include "xo/system/xo_config.h"

#if XO_PROFILER_ENABLED
#	include "xo/system/profiler.h"
#	include "xo/container/prop_node.h"
#	define XO_PROFILE_FUNCTION ::xo::scope_profiler scope_profiler_instance( __FUNCTION__ )
#	define XO_PROFILE_SCOPE( scope_name_arg ) ::xo::scope_profiler scope_profiler_instance( scope_name_arg )
inline void xo_profiler_start( const char* label = "TOTAL" ) { ::xo::profiler::instance().start( label ); }
inline ::xo::prop_node xo_profiler_report() { return ::xo::profiler::instance().report(); }
#else
#	define XO_PROFILE_FUNCTION 
#	define XO_PROFILE_SCOPE( scope_name_arg )
inline void xo_profiler_start( const char* = nullptr ) {}
inline const char* xo_profiler_report() { return ""; }
#endif
