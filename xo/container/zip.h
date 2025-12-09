#pragma once

#include <iterator>

namespace xo
{
	template< typename I1, typename I2 >
	struct zipper
	{
		using value_type = std::pair< typename std::iterator_traits<I1>::reference, typename std::iterator_traits<I2>::reference >;
		zipper( I1 b1, I1 e1, I2 b2 ) : it1( b1 ), it2( b2 ), end1( e1 ) {}
		const zipper& begin() const { return *this; }
		const zipper& end() const { return *this; }
		value_type operator*() { return value_type{ *it1, *it2 }; }
		bool operator!=( const zipper& other ) { return it1 != other.end1; }
		void operator++() { ++it1, ++it2; }

	private:
		I1 it1;
		I2 it2;
		I1 end1;
	};

	template< typename C1, typename C2 >
	auto zip( C1& c1, C2& c2 ) {
		xo_debug_assert( c1.size() <= c2.size() );
		return zipper( std::begin( c1 ), std::end( c1 ), std::begin( c2 ) );
	}
}
