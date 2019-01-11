#pragma once

namespace xo
{
	template< typename It, typename Pr > struct view_if {
		view_if( It first, It last, Pr pred ) : first_( first ), last_( last ), pred_( pred ) {}
		struct iterator : public std::iterator< std::forward_iterator_tag, typename It::value_type, typename It::value_type > {
			iterator( const view_if& cv, It pos ) : cv_( cv ), pos_( pos ) { next_match(); }
			void next_match() { while ( pos_ != cv_.last_ && !cv_.pred_( *pos_ ) ) ++pos_; }
			iterator& operator++() { ++pos_; next_match(); return *this; }
			iterator operator++( int ) { auto keepit = *this; ++pos_; next_match(); return keepit; }
			bool operator==( const iterator& other ) { return pos_ == other.pos_; }
			bool operator!=( const iterator& other ) { return pos_ != other.pos_; }
			const typename It::value_type& operator*() const { return *pos_; }
			const typename It::value_type* operator->() const { return &( *pos_ ); }
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
