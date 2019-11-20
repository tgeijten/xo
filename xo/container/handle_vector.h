#pragma once

#include "xo/xo_types.h"
#include "xo/utility/handle.h"
#include "xo/container/vector_type.h"

namespace xo
{
	/// vector that can be accessed with handles
	template< typename T, typename I = uint32 >
	class handle_vector : protected vector<T>
	{
	public:
		using typename vector<T>::value_type;
		using typename vector<T>::iterator;
		using typename vector<T>::const_iterator;
		using handle_type = typename handle<T, I>;
		using handle_span_type = typename handle_span<T, I>;
		using vector<T>::empty;
		using vector<T>::clear;
		using vector<T>::size;
		using vector<T>::capacity;
		using vector<T>::reserve;
		using vector<T>::begin;
		using vector<T>::end;
		using vector<T>::cbegin;
		using vector<T>::cend;
		using vector<T>::front;
		using vector<T>::back;
		using vector<T>::operator[];
		using vector<T>::erase;
		using vector<T>::pop_back;

		handle_vector() : vector<T>() {}

		handle_type push_back( const T& e ) { vector<T>::push_back( e ); return back_handle(); }
		handle_type push_back( T&& e ) { vector<T>::push_back( std::move( e ) ); return back_handle(); }
		template< typename... Args > handle_type emplace_back( Args&& ... args ) { vector<T>::emplace_back( std::forward<Args>( args )... ); return back_handle(); }

		const T& operator[]( handle_type h ) const { return vector<T>::operator[]( h.value() ); }
		T& operator[]( handle_type h ) { return vector<T>::operator[]( h.value() ); }

		handle_type back_handle() const { return handle_type( I( size() - 1 ) ); }
		handle_type handle_from_iterator( const_iterator it ) const { return it != end() ? handle_type( static_cast<I>( it - begin() ) ) : handle_type(); }
		typename handle_span_type::iterator span_begin() const { return typename handle_span_type::iterator( I( 0 ) ); }
		typename handle_span_type::iterator span_end() const { return typename handle_span_type::iterator( I( size() ) ); }
		handle_span_type span() const { return { span_begin(), span_end() }; }
		handle_span_type span( typename handle_span_type::iterator b ) const { return { b, span_end() }; }

		bool contains( handle_type h ) const { return h.value() < size(); }

		handle_type erase( handle_type h ) { vector<T>::erase( begin() + h.value() ); return h; }
		handle_type erase( handle_span_type hs ) { vector<T>::erase( begin() + hs.begin().value(), begin() + hs.end().value() ); return *hs.begin(); }
	};
}
