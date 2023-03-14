#pragma once

#include "xo/geometry/vec3_type.h"
#include "xo/geometry/quat_type.h"

namespace xo
{
	/// helper struct that enables stream-like access for input iterators
	template< typename It >
	struct input_iterator_stream
	{
		using value_type = typename It::value_type;
		input_iterator_stream( It pos ) : pos_( pos ) {}
		input_iterator_stream& operator>>( value_type& v ) { v = *pos_++; return *this; }
		template< typename T > input_iterator_stream& get( T& v ) { v = static_cast<T>( *pos_++ ); return *this; }
		It pos_;
	};

	/// helper struct that enables stream-like access for output iterators
	/// does not work with std::back_insert_iterator because value_type is void
	template< typename It >
	struct output_iterator_stream
	{
		using value_type = typename It::value_type;
		output_iterator_stream( It pos ) : pos_( pos ) {}
		output_iterator_stream& operator<<( value_type&& v ) { *pos_++ = std::move( v ); return *this; }
		template< typename T > output_iterator_stream& set( T& v ) { *pos_++ = static_cast<value_type>( v ); return *this; }
		It pos_;
	};

	/// helper struct that enables stream-like access for back insertion
	template< typename C >
	struct push_back_stream
	{
		using value_type = typename C::value_type;
		push_back_stream( C& cont ) : cont_( cont ) {}
		template< typename T > push_back_stream& operator<<( const T& v ) { cont_.push_back( static_cast<value_type>( v ) ); return *this; }
		push_back_stream& operator<<( value_type&& v ) { cont_.push_back( std::move( v ) ); return *this; }
		C& cont_;
	};

	/// support for convertible types
	template< typename It, typename T >
	output_iterator_stream<It>& operator<<( output_iterator_stream<It>& str, const T& v ) { return str.set( v ); }
	template< typename It, typename T >
	input_iterator_stream<It>& operator>>( input_iterator_stream<It>& str, T& v ) { return str.get( v ); }

	/// support for vec3_
	template< typename It, typename T >
	output_iterator_stream<It>& operator<<( output_iterator_stream<It>& str, const vec3_<T>& v ) {
		return str << v.x << v.y << v.z;
	}
	template< typename It, typename T >
	input_iterator_stream<It>& operator>>( input_iterator_stream<It>& str, vec3_<T>& v ) {
		return str >> v.x >> v.y >> v.z;
	}

	/// support for quat_
	template< typename It, typename T >
	output_iterator_stream<It>& operator<<( output_iterator_stream<It>& str, const quat_<T>& v ) {
		return str << v.w << v.x << v.y << v.z;
	}
	template< typename It, typename T >
	input_iterator_stream<It>& operator>>( input_iterator_stream<It>& str, quat_<T>& v ) {
		return str >> v.w >> v.x >> v.y >> v.z;
	}
}
