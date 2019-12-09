#pragma once

#include "xo/geometry/vec3_type.h"
#include "xo/geometry/quat_type.h"

namespace xo
{
	/// helper struct that enables stream-like access for input iterators
	template< typename It >
	struct istream_wrapper
	{
		using value_type = typename It::value_type;
		istream_wrapper( It pos ) : pos_( pos ) {}
		istream_wrapper& operator>>( value_type& v ) { v = *pos_++; return *this; }
		It pos_;
	};

	/// helper struct that enables stream-like access for output iterators
	/// does not work with std::back_insert_iterator because value_type is void
	template< typename It >
	struct ostream_wrapper
	{
		using value_type = typename It::value_type;
		ostream_wrapper( It pos ) : pos_( pos ) {}
		ostream_wrapper& operator<<( const value_type& v ) { *pos_++ = v; return *this; }
		ostream_wrapper& operator<<( value_type&& v ) { *pos_++ = std::move( v ); return *this; }
		It pos_;
	};

	/// helper struct that enables stream-like access for back insertion
	template< typename C >
	struct ostream_back_inserter
	{
		using value_type = typename C::value_type;
		ostream_back_inserter( C& cont ) : cont_( cont ) {}
		ostream_back_inserter& operator<<( const value_type& v ) { cont_.push_back( v ); return *this; }
		ostream_back_inserter& operator<<( value_type&& v ) { cont_.push_back( std::move( v ) ); return *this; }
		C& cont_;
	};

	/// support for vec3_
	template< typename It >
	ostream_wrapper<It>& operator<<( ostream_wrapper<It>& str, const vec3_<typename It::value_type>& v ) {
		return str << v.x << v.y << v.z;
	}
	template< typename It >
	istream_wrapper<It>& operator>>( istream_wrapper<It>& str, vec3_<typename It::value_type>& v ) {
		return str >> v.x >> v.y >> v.z;
	}

	/// support for quat_
	template< typename It >
	ostream_wrapper<It>& operator<<( ostream_wrapper<It>& str, const quat_<typename It::value_type>& v ) {
		return str << v.w << v.x << v.y << v.z;
	}
	template< typename It >
	istream_wrapper<It>& operator>>( istream_wrapper<It>& str, quat_<typename It::value_type>& v ) {
		return str >> v.w >> v.x >> v.y >> v.z;
	}
}
