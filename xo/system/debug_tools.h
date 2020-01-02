#pragma once

#define xo_do_periodic( interval_, statement_ ) \
{ static local_count_ = 0; if ( local_count_++ % interval_ == 0 ) { statement_; } }

#if XO_IS_DEBUG_BUILD
#	define xo_do_debug( statement ) statement
#else
#	define xo_do_debug( statement )
#endif
