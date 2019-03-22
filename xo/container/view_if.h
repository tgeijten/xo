#pragma once

#include <iterator>

namespace xo
{
	template< typename It, typename Pr > struct view_if {
		view_if( It first, It last, Pr pred ) : first_( first ), last_( last ), pred_( pred ) {}
		struct iterator {
			using iterator_category = typename std::iterator_traits<It>::iterator_category;
			using value_type = typename std::iterator_traits<It>::value_type;
			using difference_type = typename std::iterator_traits<It>::difference_type;
			using pointer = typename std::iterator_traits<It>::pointer;
			using reference = typename std::iterator_traits<It>::reference;

			iterator( const view_if& cv, It pos ) : cv_( cv ), pos_( pos ) { next_match(); }
			void next_match() { while ( pos_ != cv_.last_ && !cv_.pred_( *pos_ ) ) ++pos_; }
			iterator& operator++() { ++pos_; next_match(); return *this; }
			iterator operator++( int ) { auto keepit = *this; ++pos_; next_match(); return keepit; }
			bool operator==( const iterator& other ) { return pos_ == other.pos_; }
			bool operator!=( const iterator& other ) { return pos_ != other.pos_; }
			const value_type& operator*() const { return *pos_; }
			const value_type* operator->() const { return &( *pos_ ); }
			const view_if& cv_;
			It pos_;
		};
		iterator begin() const { return iterator( *this, first_ ); }
		iterator end() const { return iterator( *this, last_ ); }
		It first_, last_;
		Pr pred_;
	};

	template< typename It, typename Pr > view_if< It, Pr > make_view_if( It first, It last, Pr pred ) {
		return view_if< It, Pr >( first, last, pred );
	}

	template< typename C, typename Pr > view_if< typename C::iterator, Pr > make_view_if( C& cont, Pr pred ) {
		return view_if< typename C::iterator, Pr >( cont.begin(), cont.end(), pred );
	}
}
