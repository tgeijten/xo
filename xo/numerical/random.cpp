#include "random.h"

namespace xo
{
	XO_API random_number_generator& global_random_number_generator()
	{
		static auto global_rng = random_number_generator();
		return global_rng;
	}
}
