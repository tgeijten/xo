#pragma once

namespace xo
{
	template< size_t S, typename T > struct vec_
	{
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;

		vec_( T v = T() ) : data_{ v } {}

		/// assignment
		vec_< S, T >& operator=( const vec_< S, T >& o ) { std::copy( o.begin(), o.end(), begin() ); return *this; }

		/// element access
		const T& operator[]( index_t idx ) const { return data_[idx]; }
		T& operator[]( index_t idx ) { return data_[idx]; }
		iterator begin() { return &data_[0]; }
		iterator end() { return begin() + S; }
		const_iterator begin() const { return &data_[0]; }
		const_iterator end() const { return begin() + S; }

		T data_[S];
	};
}
