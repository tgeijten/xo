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
		using std::vector< T >::capacity;
		using std::vector< T >::reserve;
		using std::vector< T >::begin;
		using std::vector< T >::end;
		using std::vector< T >::cbegin;
		using std::vector< T >::cend;
		using std::vector< T >::front;
		using std::vector< T >::back;
		using std::vector< T >::erase;
		using std::vector< T >::data;
		using std::vector< T >::operator[];

		sorted_vector() : std::vector<T>() {}
		sorted_vector( std::initializer_list< T > l ) : std::vector<T>( l ) { std::sort( begin(), end() ); }

		iterator insert( const T& e ) {
			return std::vector< T >::insert( std::upper_bound( begin(), end(), e ), e );
		}

		iterator remove( const T& e ) {
			iterator it = find( e );
			if ( it != end() ) {
				auto it2 = it + 1;
				while ( it2 != end() && *it2 == e )
					++it2;
				return erase( it, it2 );
			}
			else return it;
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

	/// convert sorted_vector to string
	template< typename T > string to_str( const sorted_vector<T>& v ) { return container_to_str( v ); }
}
