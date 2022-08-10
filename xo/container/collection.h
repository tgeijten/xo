#pragma once

#include "xo/xo_types.h"
#include "xo/container/handle_vector.h"
#include <tuple>

namespace xo
{
	/// container that can hold multiple elements of different types
	template <typename I, typename... Ts>
	class collection
	{
	public:
		using container_type = std::tuple< handle_vector<Ts, I>... >;

		collection() { static_assert( std::is_integral_v<I>, "collection index type must be integral" ); }
		~collection() {}

		template< typename T > handle<T, I> push_back( const T& e ) { return get_vec<T>().push_back( e ); }
		template< typename T > handle<T, I> push_back( T&& e ) { return get_vec<T>().push_back( std::move( e ) ); }

		template< typename T > const T& operator[]( handle<T, I> i ) const { return get_vec<T>()[ i ]; }
		template< typename T > T& operator[]( handle<T, I> i ) { return get_vec<T>()[ i ]; }

		template< typename T > handle<T, I> erase( handle<T, I> h ) { return get_vec<T>().erase( h ); }
		template< typename T > handle<T, I> erase( handle_span<T, I> hs ) { return get_vec<T>().erase( hs ); }

		template< typename T > const handle_vector<T, I>& get_vec() const { return std::get< handle_vector<T, I> >( data_ ); }
		template< typename T > handle_vector<T, I>& get_vec() { return std::get< handle_vector<T, I> >( data_ ); }

		template< typename T > handle<T, I> get_handle( const T& e ) const {
			return handle<T, I>( static_cast<I>( &e - &get_vec<T>()[ 0 ] ) );
		}

		void clear() {
			std::apply( []( auto&... v ) { ( ( v.clear() ), ... ); }, data_ ); // fold comma expression
		}

		container_type& data() { return data_; }
		const container_type& data() const { return data_; }

	private:
		container_type data_;
	};
}
