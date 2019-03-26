#pragma once

#ifndef XO_USE_EXCEPTIONS
#	define XO_USE_EXCEPTIONS 1
#endif

#ifndef XO_USE_WINDOWS_PERFORMANCE_COUNTER
#	if defined (_MSC_VER) && (_MSC_VER <= 1800) // MSVC 2013 and lower do not have proper chrono support
#		define XO_USE_WINDOWS_PERFORMANCE_COUNTER 1
#	else
#		define XO_USE_WINDOWS_PERFORMANCE_COUNTER 0
#	endif
#endif

#if XO_USE_EXCEPTIONS
#	ifndef XO_USE_ASSERT
#		define XO_USE_ASSERT 1
#	endif
#endif

#ifndef XO_STATIC_LOG_LEVEL
#	define XO_STATIC_LOG_LEVEL trace_level
#endif
