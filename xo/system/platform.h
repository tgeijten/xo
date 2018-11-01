#pragma once

#if defined(_MSC_VER)
#	ifdef XO_EXPORTS
#		define XO_API __declspec(dllexport)
#	else
#		define XO_API __declspec(dllimport)
#	endif
#	define _CRT_SECURE_NO_WARNINGS
#	define XO_COMP_MSVC
#else
#	define XO_API
#endif

#ifdef NDEBUG
constexpr bool XO_IS_DEBUG = false;
constexpr bool XO_IS_RELEASE = true;
#else
constexpr bool XO_IS_DEBUG = true;
constexpr bool XO_IS_RELEASE = false;
#endif
