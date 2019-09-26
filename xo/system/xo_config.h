#pragma once

// DEBUG settings
#ifdef NDEBUG
#	define XO_DEBUG_MODE 0
#else
#	define XO_DEBUG_MODE 1
#endif

// Exceptions and asserts
#ifndef XO_USE_EXCEPTIONS
#	define XO_USE_EXCEPTIONS 1
#endif
#ifndef XO_USE_ASSERT
#	define XO_USE_ASSERT 1
#endif

// Log
#ifndef XO_ENABLE_LOG
#	define XO_ENABLE_LOG 1
#endif

#ifndef XO_STATIC_LOG_LEVEL
#	if XO_ENABLE_LOG
#		define XO_STATIC_LOG_LEVEL trace_level
#	else
#		define XO_STATIC_LOG_LEVEL never_log_level
#	endif
#endif

// Profiler
#ifndef XO_ENABLE_PROFILER 
#	define XO_ENABLE_PROFILER 1
#endif

// Timer
#if defined( _MSC_VER ) && !defined( XO_USE_WINDOWS_PERFORMANCE_COUNTER )
#	define XO_USE_WINDOWS_PERFORMANCE_COUNTER 1 // use windows performance counter instead of std::chrono (faster)
#endif

// MSVC warnings
#if defined( _MSC_VER )
#	ifndef XO_DISABLE_MSVC_WARNING_4251
#		define XO_DISABLE_MSVC_WARNING_4251 1 // DLL export warning
#	endif
#	ifndef XO_PROMOTE_MSVC_WARNING_5038
#		define XO_PROMOTE_MSVC_WARNING_5038 1 // Class member initialization order warning
#	endif
#endif

// MSVC settings
#if defined( _MSC_VER )
#	define XO_COMP_MSVC
#	ifdef XO_EXPORTS
#		define XO_API __declspec(dllexport)
#	else
#		define XO_API __declspec(dllimport)
#	endif
#	define _CRT_SECURE_NO_WARNINGS
#	if XO_DISABLE_MSVC_WARNING_4251
#		pragma warning( disable: 4251 )
#	endif
#	if XO_PROMOTE_MSVC_WARNING_5038
#		pragma warning( 3: 5038 )
#	endif
#else
#	define XO_API
#endif

