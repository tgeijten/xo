#pragma once

#include <cstdint>
#include <string>
#include "xo/system/platform.h"

#ifdef XO_COMP_MSVC
#	pragma warning( disable: 4307 )
#endif

namespace xo
{
	typedef std::uint64_t hash_t;

	constexpr hash_t fnv1a_64_prime = 0x100000001B3ull;
	constexpr hash_t fnv1a_64_basis = 0xCBF29CE484222325ull;

	constexpr hash_t hash_constexpr( char const* str, hash_t last_value = fnv1a_64_basis )
	{ return *str ? hash_constexpr( str + 1, ( *str ^ last_value ) * fnv1a_64_prime ) : last_value; }

	inline hash_t hash( char const* str ) {
		hash_t ret = fnv1a_64_basis;
		while ( *str ) { ret ^= *str; ret *= fnv1a_64_prime; ++str;	}
		return ret;
	}

	inline hash_t hash( const std::string& str ) { return hash( str.c_str() ); }
}

constexpr unsigned long long operator""_hash( char const* p, size_t ) { return xo::hash_constexpr( p ); }
