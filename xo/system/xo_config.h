#pragma once

/// These definitions can be added to the precompiler:
//#define XO_DISABLE_EXCEPTIONS
//#define XO_DISABLE_ASSERT
//#define XO_DISABLE_LOG
//#define XO_ENABLE_PROFILER

#if defined(_MSC_VER)
#	define XO_COMP_MSVC
#	ifdef XO_EXPORTS
#		define XO_API __declspec(dllexport)
#	else
#		define XO_API __declspec(dllimport)
#	endif
#	define _CRT_SECURE_NO_WARNINGS
#else
#	define XO_API
#endif

#ifdef NDEBUG
constexpr bool XO_IS_DEBUG_BUILD = false;
#else
#define XO_DEBUG
constexpr bool XO_IS_DEBUG_BUILD = true;
#endif

#if defined (XO_COMP_MSVC) && !defined( XO_DISABLE_WINDOWS_PERFORMANCE_COUNTER )
#	define XO_USE_WINDOWS_PERFORMANCE_COUNTER 1 // prefer windows performance counter, as it's faster than std::chrono::high_resolution_clock
#else
#	define XO_USE_WINDOWS_PERFORMANCE_COUNTER 0
#endif

#ifndef XO_STATIC_LOG_LEVEL
#	ifdef XO_DISABLE_LOG
#		define XO_STATIC_LOG_LEVEL never_log_level
#	else
#		define XO_STATIC_LOG_LEVEL trace_level
#	endif
#endif
