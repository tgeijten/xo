#include "xo/system/test_case.h"
#include "xo/utility/index.h"

namespace xo
{
	XO_TEST_CASE( xo_index_test )
	{
		std::vector<double> v{ 1.0, 2.0, 3.0, 4.0 };
		auto idx = xo::index_class<double>( 1 );

		XO_CHECK( v[idx.idx] == 2.0 );
		XO_CHECK( v[( idx + 1 ).idx] == 3.0 );
		auto i3 = idx + 2;
		XO_CHECK( v[i3.idx] == 4.0 );

		index_span sp = { xo::index_class<double>( 3 ), xo::index_class<double>( 9 ) };
		auto icheck = *sp.begin();
		XO_CHECK( sp.size() == 6 );
		XO_CHECK( !sp.empty() );
		for ( auto i : sp )
			XO_CHECK( i == icheck++ );

		auto idx2 = idx.as<int>();
		XO_CHECK( idx.idx == idx2.idx );
	}
}
