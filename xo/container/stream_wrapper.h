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
		It pos_;
	};

	/// helper struct that enables stream-like access for output iterators
	/// does not work with std::back_insert_iterator because value_type is void
	template< typename It >
	struct output_iterator_stream
	{
		using value_type = typename It::value_type;
		output_iterator_stream( It pos ) : pos_( pos ) {}
		output_iterator_stream& operator<<( const value_type& v ) { *pos_++ = v; return *this; }
		output_iterator_stream& operator<<( value_type&& v ) { *pos_++ = std::move( v ); return *this; }
		It pos_;
	};

	/// helper struct that enables stream-like access for back insertion
	template< typename C >
	struct push_back_stream
	{
		using value_type = typename C::value_type;
		push_back_stream( C& cont ) : cont_( cont ) {}
		push_back_stream& operator<<( const value_type& v ) { cont_.push_back( v ); return *this; }
		push_back_stream& operator<<( value_type&& v ) { cont_.push_back( std::move( v ) ); return *this; }
		C& cont_;
	};

	/// support for vec3_
	template< typename It >
	output_iterator_stream<It>& operator<<( output_iterator_stream<It>& str, const vec3_<typename It::value_type>& v ) {
		return str << v.x << v.y << v.z;
	}
	template< typename It >
	input_iterator_stream<It>& operator>>( input_iterator_stream<It>& str, vec3_<typename It::value_type>& v ) {
		return str >> v.x >> v.y >> v.z;
	}

	/// support for quat_
	template< typename It >
	output_iterator_stream<It>& operator<<( output_iterator_stream<It>& str, const quat_<typename It::value_type>& v ) {
		return str << v.w << v.x << v.y << v.z;
	}
	template< typename It >
	input_iterator_stream<It>& operator>>( input_iterator_stream<It>& str, quat_<typename It::value_type>& v ) {
		return str >> v.w >> v.x >> v.y >> v.z;
	}
}
