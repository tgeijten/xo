#pragma once

#include "xo/xo_types.h"
#include "xo/utility/index.h"
#include <tuple>
#include <vector>

namespace xo
{
	/// container that can hold multiple elements of different types
	template <typename I, typename... Ts>
	class collection_ic
	{
	public:
		using container_type = std::tuple< std::vector<Ts>... >;

		collection_ic() { static_assert( std::is_integral_v<I>, "collection_ic index type must be integral" ); }
		~collection_ic() {}

		template< typename T > index_class<T, I> push_back( const T& e ) { get_vec<T>().push_back( e ); return back_index<T>(); }
		template< typename T > index_class<T, I> push_back( T&& e ) { get_vec<T>().push_back( std::move( e ) ); return back_index<T>(); }

		template< typename T > const T& operator[]( index_class<T, I> i ) const { return get_vec<T>()[ i.idx ]; }
		template< typename T > T& operator[]( index_class<T, I> i ) { return get_vec<T>()[ i.idx ]; }

		template< typename T > index_class<T, I> erase( index_class<T, I> i ) { auto& v = get_vec<T>(); v.erase( i.iter( v ) ); }
		//template< typename T > index_class<T, I> erase( index_class<T, I> b, index_class<T, I> e ) { return get_vec<T>().erase( i ); }

		template< typename T > const std::vector<T>& get_vec() const { return std::get< std::vector<T> >( data_ ); }
		template< typename T > std::vector<T>& get_vec() { return std::get< std::vector<T> >( data_ ); }

		template< typename T > auto begin() const { return get_vec<T>().begin(); }
		template< typename T > auto begin() { return get_vec<T>().begin(); }
		template< typename T > auto end() const { return get_vec<T>().end(); }
		template< typename T > auto end() { return get_vec<T>().end(); }

		/// Get index of element in container. Behavior is not defined when e is not part of container.
		template< typename T > index_class<T, I> get_index( const T& e ) const { return index_class<T, I>( I( &e - &get_vec<T>()[ 0 ] ) ); }

		template< typename T > index_class<T, I> back_index() const { return index_class<T, I>::back( get_vec<T>() ); }
		template< typename T > index_class<T, I> end_index() const { return index_class<T, I>::end( get_vec<T>() ); }

		void clear() {
			std::apply( []( auto&... v ) { ( ( v.clear() ), ... ); }, data_ ); // fold comma expression
		}

		container_type& data() { return data_; }
		const container_type& data() const { return data_; }

	private:
		container_type data_;
	};
}
