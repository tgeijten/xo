#pragma once

namespace xo
{
	template< typename T, size_t N >
	class array
	{
	public:
		using value_type = T;
		using size_type = size_t;
		using iterator = T*;
		using const_iterator = const T*;

		T& operator[]( size_t i ) { return data_[ i ]; }
		const T& operator[]( size_t i ) const { return data_[ i ]; }

		iterator begin() { return &data_[ 0 ]; }
		const_iterator begin() const { return &data_[ 0 ]; }
		const_iterator cbegin() const { return &data_[ 0 ]; }

		iterator end() { return &data_[ N ]; }
		const_iterator end() const { return &data_[ N ]; }
		const_iterator cend() const { return &data_[ N ]; }

		constexpr size_t size() const { return N; }

		T data_[ N ];
	};
}
