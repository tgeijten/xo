#pragma once

#include "xo/xo_types.h"

namespace xo
{
	template< typename T >
	struct average_
	{
		average_() : tot_v( T() ), tot_w( T() ) {}
		~average_() {}

		void add( T value, T w = T(1) ) { tot_v += w * value; tot_w += w; }
		T get() { return tot_w > T() ? tot_v / tot_w : T(); }
		bool empty() { return tot_w == T(); }
		void reset() { tot_v = tot_w = T(); }

	private:
		T tot_v;
		T tot_w;
	};

	using averagef = average_< float >;
	using averaged = average_< double >;
}
