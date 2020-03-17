#pragma once

// DEBUG settings
#ifdef NDEBUG
#	define XO_IS_DEBUG_BUILD 0
#else
#	define XO_IS_DEBUG_BUILD 1
#endif

// Exceptions and asserts
#ifndef XO_DISABLE_EXCEPTIONS
#	define XO_EXCEPTIONS_ENABLED 1
#else
#	define XO_EXCEPTIONS_ENABLED 0
#endif

#ifndef XO_DISABLE_ASSERT
#	define XO_ASSERT_ENABLED 1
#else
#	define XO_ASSERT_ENABLED 0
#endif

// Log
#ifndef XO_DISABLE_LOG
#	define XO_LOG_ENABLED 1
#	if defined( XO_STATIC_LOG_LEVEL_TRACE )
#		define XO_STATIC_LOG_LEVEL level::trace
#	elif defined( XO_STATIC_LOG_LEVEL_DEBUG )
#		define XO_STATIC_LOG_LEVEL level::debug
#	elif defined( XO_STATIC_LOG_LEVEL_INFO )
#		define XO_STATIC_LOG_LEVEL level::info
#	elif defined( XO_STATIC_LOG_LEVEL_WARNING )
#		define XO_STATIC_LOG_LEVEL level::warning
#	elif defined( XO_STATIC_LOG_LEVEL_ERROR )
#		define XO_STATIC_LOG_LEVEL level::error
#	elif defined( XO_STATIC_LOG_LEVEL_CRITICAL )
#		define XO_STATIC_LOG_LEVEL level::critical
#	else
#		define XO_STATIC_LOG_LEVEL level::trace
#	endif
#else
#	define XO_LOG_ENABLED 0
#	define XO_STATIC_LOG_LEVEL level::never
#endif

// Profiler
#ifndef XO_DISABLE_PROFILER
#	define XO_PROFILER_ENABLED 1
#else
#	define XO_PROFILER_ENABLED 0
#endif

// MSVC settings
#if defined( _MSC_VER )
#	define XO_COMP_MSVC

	// dll export
#	ifdef XO_EXPORTS
#		define XO_API __declspec(dllexport)
#	else
#		define XO_API __declspec(dllimport)
#	endif

	// windows performance counter
#	ifndef XO_DISABLE_WINDOWS_PERFORMANCE_COUNTER
#		define XO_WINDOWS_PERFORMANCE_COUNTER_ENABLED 1 // use windows performance counter for timing (faster)
#	else
#		define XO_WINDOWS_PERFORMANCE_COUNTER_ENABLED 0 // use std::chono for timing
#	endif

	// MSVC specific warnings
#	ifndef XO_ENABLE_MSVC_WARNINGS
#		pragma warning( disable: 4251 ) // DLL export warning is disabled by default
#		define _CRT_SECURE_NO_WARNINGS
#	endif

#	ifndef XO_DISABLE_MSVC_WARNING_5038
#		pragma warning( 3: 5038 ) // Class member initialization order warning
#	endif

#else
#	define XO_API
#endif
