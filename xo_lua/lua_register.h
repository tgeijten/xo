#pragma once

#include "xo/geometry/vec3.h"
#include "xo/geometry/quat.h"
#include <string>
#include "sol/sol.hpp"

namespace xo
{
	template< typename T >
	void lua_register_vec3( sol::state& lua, const std::string& name = "vec3" )
	{
		using vec3 = vec3_<T>;

		auto vec3_mul_overloads = sol::overload(
			[]( vec3& v, T d ) -> vec3 { return v * d; },
			[]( T d, vec3& v ) -> vec3 { return d * v; }
		);

		lua.new_usertype<vec3>( name, sol::constructors<vec3( T, T, T )>(),
			"x", &vec3::x,
			"y", &vec3::y,
			"z", &vec3::z,
			sol::meta_function::to_string, []( vec3& v ) -> std::string { return to_str( v ); },
			sol::meta_function::addition, []( vec3& v1, vec3& v2 ) -> vec3 { return v1 + v2; },
			sol::meta_function::subtraction, []( vec3& v1, vec3& v2 ) -> vec3 { return v1 - v2; },
			sol::meta_function::multiplication, vec3_mul_overloads,
			sol::meta_function::division, []( vec3& v, double d ) -> vec3 { return v / d; },
			sol::meta_function::unary_minus, []( vec3& v ) -> vec3 { return -v; },
			sol::meta_function::equal_to, []( vec3& v1, vec3& v2 ) -> bool { return v1 == v2; },
			"length", []( vec3& v ) -> T { return length( v ); },
			"squared_length", []( vec3& v ) -> T { return squared_length( v ); },
			"is_null", &vec3::is_null,
			"clear", &vec3::clear,
			"normalize", []( vec3& v ) -> T { return normalize( v ); },
			"normalized", []( vec3& v ) -> vec3 { return normalized( v ); },
			"cross_product", []( vec3& v1, vec3& v2 ) -> vec3 { return cross_product( v1, v2 ); },
			"dot_product", []( vec3& v1, vec3& v2 ) -> T { return dot_product( v1, v2 ); }
		);
	}

	template< typename T >
	void lua_register_quat( sol::state& lua, const std::string& name = "quat" )
	{
		using quat = quat_<T>;
		using vec3 = vec3_<T>;

		auto quat_mul_overloads = sol::overload(
			[]( quat& q1, quat& q2 ) -> quat { return q1 * q2; },
			[]( quat& q, vec3& v ) -> vec3 { return q * v; }
		);

		lua.new_usertype<quat>( name, sol::constructors<quat( T, T, T, T )>(),
			"w", &quat::w,
			"x", &quat::x,
			"y", &quat::y,
			"z", &quat::z,
			sol::meta_function::to_string, []( quat& q ) -> std::string { return to_str( q ); },
			sol::meta_function::multiplication, quat_mul_overloads,
			sol::meta_function::unary_minus, []( quat& q ) -> quat { return conjugate( q ); },
			"length", []( quat& q ) -> double { return length( q ); },
			"squared_length", []( quat& q ) -> double { return squared_length( q ); },
			"conjugate", []( quat& q ) -> quat { return conjugate( q ); },
			"inverse", []( quat& q ) -> quat { return inverse( q ); },
			"normalize", []( quat& q ) -> T { return normalize( q ); },
			"normalized", []( quat& q ) -> quat { return normalized( q ); },
			"rotation_vector", []( quat& q ) -> vec3 { return rotation_vector_from_quat( q ); }
			);
	}
}
