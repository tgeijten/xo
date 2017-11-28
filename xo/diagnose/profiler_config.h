#pragma once

#ifdef XO_ENABLE_PROFILER
#	include "xo/system/profiler.h"
#	define XO_PROFILE_FUNCTION ::xo::scoped_section_profiler _PROFILE_SECTION_( __FUNCTION__ )
#	define XO_PROFILE_SCOPE( scope_name_arg ) ::xo::scoped_section_profiler _PROFILE_SECTION_( scope_name_arg )
inline void xo_profiler_reset() { ::xo::profiler::instance().reset(); }
inline void xo_profiler_log_report( ::xo::log::level l ) { ::xo::log::message( l, ::xo::profiler::instance().report() ); }
#else
#	define XO_PROFILE_FUNCTION
#	define XO_PROFILE_SCOPE( scope_name_arg )
inline void xo_profiler_reset() {}
inline void xo_profiler_log_report( ::xo::log::level l ) {}
#endif
