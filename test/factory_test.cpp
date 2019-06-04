#pragma once

#include "xo/container/prop_node.h"
#include "xo/utility/factory.h"
#include "xo/system/test_framework.h"

namespace xo
{
	struct FA { FA( const prop_node& pn ) : value( 0 ) {} int value; };
	struct FB : FA { FB( const prop_node& pn ) : FA( pn ) { value = 67; } };
	struct FC : FA { FC( const prop_node& pn ) : FA( pn ) { value = -1; } };

	XO_TEST_CASE( xo_factory_test )
	{
		factory< FA, prop_node > fact;
		fact.register_type< FB >( "B" );
		fact.register_type< FC >( "C" );

		prop_node pn;

		auto b = fact.create( "B", pn );
		auto c = fact.create( "C", pn );

		XO_CHECK( b->value == 67 );
		XO_CHECK( c->value == -1 );
	}
}
