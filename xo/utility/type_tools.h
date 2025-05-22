#pragma once

#include <type_traits>

namespace xo
{
	template<typename T, typename... Ts>
	struct is_one_of : std::disjunction<std::is_same<T, Ts>...> {};

	template<typename T, typename... Ts>
	inline constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;
}
