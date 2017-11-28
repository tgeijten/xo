#pragma once

#include "xo/system/assert.h"
#include <numeric>

namespace xo
{
	template< typename C > typename C::iterator find( C& cont, typename C::value_type& e )
	{ return std::find( std::begin( cont ), std::end( cont ), e ); }

	template< typename C, typename P > typename C::const_iterator find( const C& cont, typename C::value_type& e )
	{ return std::find( std::begin( cont ), std::end( cont ), e ); }

	template< typename C, typename P > typename C::iterator find_if( C& cont, P pred )
	{ return std::find_if( std::begin( cont ), std::end( cont ), pred ); }

	template< typename C, typename P > typename C::const_iterator find_if( const C& cont, const P pred )
	{ return std::find_if( std::cbegin( cont ), std::cend( cont ), pred ); }

	template< typename C > typename C::iterator min_element( C& cont )
	{ return std::min_element( std::begin( cont ), std::end( cont ) ); }

	template< typename C > typename C::const_iterator min_element( const C& cont )
	{ return std::min_element( std::cbegin( cont ), std::cend( cont ) ); }

	template< typename C > typename C::iterator max_element( C& cont )
	{ return std::max_element( std::begin( cont ), std::end( cont ) ); }

	template< typename C > typename C::const_iterator max_element( const C& cont )
	{ return std::max_element( std::cbegin( cont ), std::cend( cont ) ); }

	template< typename I > typename I::value_type average( I b, I e )
	{ return std::accumulate( b, e, I::value_type( 0 ) ) / typename I::value_type( e - b ); }

	template< typename C > typename C::value_type average( const C& cont )
	{ return average( std::begin( cont ), std::end( cont ) ); }

	template < typename C > vector< index_t > sort_indices( const C& cont ) {
		vector< size_t > idx_vec( cont.size() );
		for ( index_t i = 0; i < cont.size(); ++i ) idx_vec[ i ] = i;
		sort( idx_vec.begin(), idx_vec.end(), [ &cont ]( size_t i1, size_t i2 ) { return cont[ i1 ] < cont[ i2 ]; } );
		return idx_vec;
	}

	template< typename C > C sorted_copy( const C& cont ) { C res( cont ); std::sort( res.begin(), res.end() ); return res; }

	template< typename T > T median( const vector< T >& vec ) {
		xo_assert( vec.size() > 0 );
		auto s = vec.size();
		auto v = sorted_copy( vec );
		if ( s % 2 == 1 ) return v[ s / 2 ];
		else return ( v[ s / 2 ] + v[ s / 2 - 1 ] ) / T(2);
	}

	template< typename T > T top_average( const vector< T >& vec, size_t count ) {
		vector< T > result( count );
		std::partial_sort_copy( vec.begin(), vec.end(), result.begin(), result.end() );
		return average( result );
	}

	template< typename T > index_t find_index( const vector< T >& vec, const T& val ) {
		auto it = std::find( vec.begin(), vec.end(), val );
		if ( it == vec.end() ) return no_index;
		else return it - vec.begin();
	}

	template< typename T > index_t back_index( const vector< T >& vec )
	{ xo_assert( vec.size() > 0 ) return vec.size() - 1; }

	template< typename It, typename Pr > struct view_if {
		view_if( It first, It last, Pr pred ) : first_( first ), last_( last ), pred_( pred ) {}
		struct iterator : public std::iterator< std::forward_iterator_tag, typename It::value_type, typename It::value_type > {
			iterator( const view_if& cv, It pos ) : cv_( cv ), pos_( pos ) { next_match(); }
			void next_match() { while ( pos_ != cv_.last_ && !cv_.pred_( *pos_ ) ) ++pos_; }
			iterator& operator++() { ++pos_; next_match(); return *this; }
			iterator operator++( int ) { auto keepit = *this; ++pos_; next_match(); return keepit; }
			bool operator==( const iterator& other ) { return pos_ == other.pos_; }
			bool operator!=( const iterator& other ) { return pos_ != other.pos_; }
			typename const It::value_type& operator*() const { return *pos_; }
			typename const It::value_type* operator->() const { return &(*pos_); }
			const view_if& cv_;
			It pos_;
		};
		iterator begin() const { return iterator( *this, first_ ); }
		iterator end() const { return iterator( *this, last_ ); }
		It first_, last_;
		Pr pred_;
	};

	template< typename T > std::ostream& operator<<( std::ostream& str, const vector< T >& vec )
	{
		for ( auto it = vec.begin(); it != vec.end(); ++it ) {
			if ( it != vec.begin() ) str << "\t";
			str << *it;
		}
		return str << std::endl;
	}

	template< typename It, typename Pr > view_if< It, Pr > make_view_if( It first, It last, Pr pred ) {
		return view_if< It, Pr >( first, last, pred );
	}

	template< typename C, typename Pr > view_if< typename C::iterator, Pr > make_view_if( C& cont, Pr pred ) {
		return view_if< typename C::iterator, Pr >( cont.begin(), cont.end(), pred );
	}
}
