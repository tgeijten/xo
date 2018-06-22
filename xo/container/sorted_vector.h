#pragma once

namespace xo
{
	template< typename T >
	class sorted_vector : private std::vector< T >
	{
	public:
		using typename std::vector< T >::value_type;
		using typename std::vector< T >::iterator;
		using typename std::vector< T >::const_iterator;

		using std::vector< T >::empty;
		using std::vector< T >::clear;
		using std::vector< T >::size;
		using std::vector< T >::begin;
		using std::vector< T >::end;
		using std::vector< T >::cbegin;
		using std::vector< T >::cend;
		using std::vector< T >::front;
		using std::vector< T >::back;
		using std::vector< T >::erase;

		iterator insert( const T& e ) {
			return std::vector< T >::insert( std::upper_bound( begin(), end(), e ), e );
		}

		iterator find( const T& e ) {
			iterator it = std::lower_bound( begin(), end(), e );
			return ( it != end() && *it == e ) ? it : end();
		}

		const_iterator find( const T& e ) const {
			const_iterator it = std::lower_bound( cbegin(), cend(), e );
			return ( it != cend() && *it == e ) ? it : cend();
		}
	};
}
