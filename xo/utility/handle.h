#pragma once

#include "xo/xo_types.h"

namespace xo
{
	/// non-owning handle to a specific object, typically an index
	template< typename T, typename I = uint32 >
	struct handle
	{
		using object_type = T;
		using id_type = I;

		constexpr handle() : value_( invalid_id() ) {}
		explicit constexpr handle( id_type v ) : value_( v ) {}

		explicit operator bool() const { return value_ != invalid_id(); }

		const id_type& value() const { return value_; }
		id_type& value() { return value_; }

		void reset() { value_ = invalid_id(); }
		void swap( handle& o ) { id_type tmp = value_; value_ = o.value_; o.value_ = tmp; }

		friend bool operator<( const handle a, const handle b ) { return a.value_ < b.value_; }
		friend bool operator==( const handle a, const handle b ) { return a.value_ == b.value_; }
		friend bool operator!=( const handle a, const handle b ) { return a.value_ != b.value_; }

	private:
		static constexpr id_type invalid_id() { return ~id_type( 0 ); }
		id_type value_;
	};

	/// continuous span of handles
	template< typename T, typename I = uint32 >
	struct handle_span
	{
		using handle_type = handle<T, I>;

		/// iterator through a continuous span of handles
		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = handle_type;
			using difference_type = I;
			using pointer = handle_type *;
			using reference = handle_type &;
			iterator( handle_type v ) : handle_( v ) {}
			iterator( I v ) : handle_( v ) {}
			iterator& operator++() { ++handle_.value(); return *this; }
			iterator operator++( int ) { auto h = *this; ++handle_.value(); return h; }
			bool operator==( const iterator& o ) const { return o.handle_ == handle_; }
			bool operator!=( const iterator& o ) const { return o.handle_ != handle_; }
			const handle_type& operator*() const { return handle_; }
			const I& value() const { return handle_.value(); }
			handle_type handle_;
		};

		constexpr handle_span() : begin_(), end_() {}
		constexpr handle_span( handle_type b, handle_type e ) : begin_( b ), end_( e ) {}
		constexpr handle_span( iterator b, iterator e ) : begin_( b ), end_( e ) {}

		bool empty() const { return begin_ == end_; }
		I size() const { return end_.value() - begin_.value(); }
		bool contains( handle_type h ) const { return h.value() >= begin_.value() && h.value() < end_.value(); }
		bool is_beyond( handle_type h ) const { return h.value() >= end_.value(); }

		iterator begin() const { return begin_; }
		iterator end() const { return end_; }

	private:
		iterator begin_;
		iterator end_;
	};

	/// update handle after elements have been erased
	template< typename T, typename I = uint32 >
	handle<T, I>& update_moved_handle( handle<T, I>& h, const handle_span<T, I>& moved_handles, const typename handle_span<T, I>::iterator new_begin ) {
		if ( moved_handles.contains( h ) ) {
			h.value() = h.value() - moved_handles.begin().value() + new_begin.value();
		}
		return h;
	}
}
