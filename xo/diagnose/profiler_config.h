#pragma once

#include "xo/system/log_level.h"

#ifdef XO_ENABLE_PROFILER
#	include "xo/diagnose/profiler.h"
#	define XO_PROFILE_FUNCTION ::xo::scoped_section_profiler _PROFILE_SECTION_( __FUNCTION__ )
#	define XO_PROFILE_SCOPE( scope_name_arg ) ::xo::scoped_section_profiler _PROFILE_SECTION_( scope_name_arg )
inline void xo_profiler_reset() { ::xo::profiler::instance().reset(); }
inline ::xo::prop_node xo_profiler_report() { return ::xo::profiler::instance().report(); }
#else
#	define XO_PROFILE_FUNCTION 
#	define XO_PROFILE_SCOPE( scope_name_arg )
inline void xo_profiler_reset() {}
inline const char* xo_profiler_report() { return ""; }
#endif
