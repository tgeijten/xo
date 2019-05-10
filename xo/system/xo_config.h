#pragma once

//#define XO_DISABLE_EXCEPTIONS // define to disable exceptions
//#define XO_DISABLE_ASSERT // define to disable xo_assert
//#define XO_DISABLE_LOG // define to disable all logging
#define XO_ENABLE_PROFILER // define to enable global profiler
//#define XO_DISABLE_WINDOWS_PERFORMANCE_COUNTER // define to use std::chrono on MSVC
//#define XO_ENABLE_MSVC_WARNING_4251 // define to not disable warning 4251 on MSVC

#if defined(_MSC_VER)
#	define XO_COMP_MSVC
#	ifdef XO_EXPORTS
#		define XO_API __declspec(dllexport)
#	else
#		define XO_API __declspec(dllimport)
#	endif
#	define _CRT_SECURE_NO_WARNINGS
#	ifndef XO_ENABLE_MSVC_WARNING_4251
#		pragma warning( disable: 4251 ) // disable W4251, unfortunately there's no nice way to do this
#	endif
#else
#	define XO_API
#endif

#ifdef NDEBUG
#	define XO_IS_DEBUG_BUILD false
#else
#	define XO_DEBUG
#	define XO_IS_DEBUG_BUILD true
#endif

#if defined (XO_COMP_MSVC) && !defined( XO_DISABLE_WINDOWS_PERFORMANCE_COUNTER )
#	define XO_USE_WINDOWS_PERFORMANCE_COUNTER 1 // use windows performance counter for timing (faster)
#else
#	define XO_USE_WINDOWS_PERFORMANCE_COUNTER 0 // use std::chono for timing (slower)
#endif

#ifndef XO_STATIC_LOG_LEVEL
#	ifdef XO_DISABLE_LOG
#		define XO_STATIC_LOG_LEVEL never_log_level
#	else
#		define XO_STATIC_LOG_LEVEL trace_level
#	endif
#endif
