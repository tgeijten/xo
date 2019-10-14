#pragma once

#include "xo/xo_types.h"
#include "xo/container/handle_vector.h"
#include <tuple>

namespace xo
{
	template <typename... Ts>
	class collection
	{
	public:
		using container_type = std::tuple< handle_vector<Ts>... >;

		collection() : data_() {}
		~collection() {}

		template< typename T > handle<T> push_back( const T& e ) { return get_vec<T>().push_back( e ); }
		template< typename T > handle<T> push_back( T&& e ) { return get_vec<T>().push_back( std::move( e ) ); }

		template< typename T > const T& operator[]( handle<T> i ) const { return get_vec<T>()[ i ]; }
		template< typename T > T& operator[]( handle<T> i ) { return get_vec<T>()[ i ]; }

		template< typename T > handle<T> erase( const T& e ) { return get_vec<T>().erase( e ); }
		template< typename T > void pop_back( const T& e ) { get_vec<T>().pop_back( e ); }

		template< typename T > const handle_vector<T>& get_vec() const { return std::get< handle_vector<T> >( data_ ); }
		template< typename T > handle_vector<T>& get_vec() { return std::get< handle_vector<T> >( data_ ); }

		void clear() {
			std::apply( []( auto&... v ) { ( ( v.clear() ), ... ); }, data_ ); // fold comma expression
		}

		container_type& data() { return data_; }
		const container_type& data() const { return data_; }

	private:
		container_type data_;
	};
}
