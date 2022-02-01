#pragma once

#include "xo/xo_types.h"
#include <iterator>
#include <utility>

namespace xo
{
	/// non-owning handle to a specific object, typically an index
	template< typename T, typename I = uint32 >
	struct handle
	{
		using object_type = T;
		using id_type = I;

		// default, copy and move constructor
		constexpr handle() : value_( invalid_id() ) {}
		handle( const handle& o ) = default;
		handle( handle&& o ) noexcept : value_( o.value_ ) { o.reset(); }

		// explicit conversion constructors
		explicit constexpr handle( id_type v ) : value_( v ) {}
		template<typename U> explicit constexpr handle( handle<U, I> o ) : value_( o.value() ) {}

		// default copy and move assignment
		handle& operator=( const handle& o ) { value_ = o.value_; return *this; }
		handle& operator=( handle&& o ) noexcept { std::swap( value_, o.value_ ); return *this; }

		explicit operator bool() const { return value_ != invalid_id(); }

		const id_type& value() const { return value_; }
		id_type& value() { return value_; }

		void reset() { value_ = invalid_id(); }

		friend bool operator<( const handle a, const handle b ) { return a.value_ < b.value_; }
		friend bool operator==( const handle a, const handle b ) { return a.value_ == b.value_; }
		friend bool operator!=( const handle a, const handle b ) { return a.value_ != b.value_; }

		template< typename C > auto iter( C& c ) const { return c.begin() + value_; }
		template< typename C > auto iter( const C& c ) const { return c.begin() + value_; }

		template< typename C > static handle end( const C& c ) { return handle( id_type( c.size() ) ); }
		template< typename C > static handle back( const C& c ) { return handle( id_type( c.size() ) - 1 ); }

	protected:
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
			using pointer = handle_type*;
			using reference = handle_type&;
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
