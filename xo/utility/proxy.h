#pragma once

#include "xo/system/assert.h"
#include "xo/container/indexed_set.h"

namespace xo
{
	template< typename T, typename I = uint32_t >
	struct proxy
	{
		using value_t = typename T;
		using index_t = typename I;

		proxy() : index_( data_.get_or_add( value_t() ) ) {}
		proxy( const value_t& value ) : index_( data_.get_or_add( value ) ) {}

		proxy& set( const value_t& value ) { index_ = data_.get_or_add( value ); return *this; }
		proxy& operator=( const value_t& value ) { return set( value ); }

		const value_t& get() const { return data_[ index_ ]; }
		operator const value_t&() const { return get(); }

		friend bool operator==( const proxy a, const proxy b ) { return a.index_ == b.index_; }
		friend bool operator!=( const proxy a, const proxy b ) { return a.index_ != b.index_; }

	private:
		static inline indexed_set< T, I > data_ = {};
		index_t index_;
	};
}
