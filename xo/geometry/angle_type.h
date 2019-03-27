#pragma once

namespace xo
{
	enum class angle_unit { degrees, radians };
	template< angle_unit U, typename T > struct angle_;
}
