#include "interruptible.h"

namespace xo
{
	interruptible::interruptible() : interrupt_flag_( false )
	{}

	interruptible::~interruptible()
	{}

	void interruptible::interrupt() const
	{
		interrupt_flag_ = true;
	}

	bool interruptible::test_interrupt_flag() const
	{
		return interrupt_flag_;
	}
}
