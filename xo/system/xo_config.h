#pragma once

#ifndef XO_USE_EXCEPTIONS
#	define XO_USE_EXCEPTIONS 1
#endif

#if XO_USE_EXCEPTIONS
#	ifndef XO_USE_ASSERT
#		define XO_USE_ASSERT 1
#	endif
#endif

#ifndef XO_STATIC_LOG_LEVEL
#	define XO_STATIC_LOG_LEVEL trace_level
#endif
