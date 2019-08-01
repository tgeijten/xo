#pragma once

#include "xo/xo_types.h"
#include "xo/utility/handle.h"
#include <vector>

namespace xo
{
	/// vector that can be accessed with handles
	template< typename T, typename I = uint32_t >
	class handle_vector : protected std::vector<T>
	{
	public:
		using typename std::vector<T>::value_type;
		using typename std::vector<T>::iterator;
		using typename std::vector<T>::const_iterator;
		using handle_t = typename handle<T, I>;
		using std::vector<T>::empty;
		using std::vector<T>::clear;
		using std::vector<T>::size;
		using std::vector<T>::capacity;
		using std::vector<T>::reserve;
		using std::vector<T>::begin;
		using std::vector<T>::end;
		using std::vector<T>::cbegin;
		using std::vector<T>::cend;
		using std::vector<T>::front;
		using std::vector<T>::back;
		using std::vector<T>::erase;

		handle_vector() : std::vector<T>() {}

		handle_t push_back( const T& e ) {
			std::vector<T>::push_back( e );
			return handle_t( size() - 1 );
		}
		handle_t push_back( T&& e ) {
			std::vector<T>::push_back( std::move( e ) );
			return handle_t( size() - 1 );
		}

		template< typename... Args > handle_t emplace_back( Args&&... args ) {
			std::vector<T>::emplace_back( std::forward<Args>( args )... );
			return handle_t( size() - 1 );
		}

		const T& operator[]( handle_t i ) const { return std::vector<T>::operator[]( i.value ); }
		T& operator[]( handle_t i ) { return std::vector<T>::operator[]( i.value ); }

		handle_t find( const T& e ) {
			for ( handle_t h = 0; h.value < size(); ++h.value )
				if ( ( *this )[ i ] == e )
					return h;
			return handle_t();
		}
	};
}
