#pragma once

#include "xo/container/storage.h"
#include "xo/utility/data_algorithms.h"
#include <iostream>

namespace xo
{
	void data_test()
	{
		storage< double > sto( 0, 4 );
		sto.add_frame( { 5, 0, 3, 7 } );
		sto.add_frame( { 1, -5, 7, 3 } );
		sto.add_frame( { 4, 9, 8, 10 } );
		auto t = compute_covariance( sto );

		storage< double > sto2( 0, 3 );
		sto2.add_frame( { 90, 60, 90 } );
		sto2.add_frame( { 90, 90, 30 } );
		sto2.add_frame( { 60, 60, 60 } );
		sto2.add_frame( { 60, 60, 90 } );
		sto2.add_frame( { 30, 30, 30 } );
		auto t2 = compute_covariance( sto2 );
	}
}
