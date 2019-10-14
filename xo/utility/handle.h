#pragma once

#include "xo/system/assert.h"
#include "xo/xo_types.h"
#include "xo/numerical/math.h"

namespace xo
{
	/// non-owning handle to a specific object, typically an index
	template< typename T, typename I = uint32 >
	struct handle
	{
		using id_type = I;

		constexpr handle() : value_( invalid_id() ) {}
		explicit constexpr handle( id_type v ) : value_( v ) {}

		explicit operator bool() const { return value_ != invalid_id(); }

		const id_type& value() const { return value_; }
		id_type& value() { return value_; }

		void reset() { value_ = invalid_id(); }
		void swap( handle& o ) { id_type tmp = value_; value_ = o.value_; o.value_ = tmp; }

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
		constexpr handle_span() : begin_(), end_() {}
		constexpr handle_span( handle_type b, handle_type e ) : begin_( b ), end_( e ) {}

		bool empty() const { return begin_ == end_; }
		I size() const { return end_.value() - begin_.value(); }
		bool contains( handle_type h ) const { return h.value() >= begin_.value() && h.value() < end_.value(); }

		handle_type begin() const { return begin_; }
		handle_type end() const { return end_; }

	private:
		handle_type begin_;
		handle_type end_;
	};

	/// update handle after elements have been erased
	template< typename T, typename I = uint32 >
	handle<T, I>& consolidate( handle<T, I>& h, handle_span<T, I> hs ) {
		if ( h.value() >= hs.end().value() )
			h.value() -= hs.size();
	}
}
