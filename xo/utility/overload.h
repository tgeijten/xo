#pragma once

namespace xo
{
	template<typename ...Ts>
	struct overload : Ts...
	{
		overload( const Ts& ... args ) : Ts( args )...
		{}
		using Ts::operator()...;
	};
}
