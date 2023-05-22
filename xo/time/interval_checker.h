#pragma once

#include "time.h"

namespace xo
{
	struct interval_checker
	{
		interval_checker( time interval ) : interval_( interval ), previous_time_() {}
		bool check( time t ) {
			if ( t - previous_time_ >= interval_ ) {
				previous_time_ = t;
				return true;
			}
			else return false;
		}
	private:
		time interval_;
		time previous_time_;
	};
}

