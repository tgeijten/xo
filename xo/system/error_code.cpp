#include "error_code.h"
#include "log.h"

namespace xo
{
	bool try_set_error( error_code* ec, string msg )
	{
		log::error( msg );
		if ( ec )
			ec->set( -1, msg );
		return ec != nullptr;
	}

	void set_error_or_throw( error_code* ec, string msg )
	{
		log::error( msg );
		if ( ec )
			ec->set( -1, msg );
		else xo_error( msg );
	}
}
