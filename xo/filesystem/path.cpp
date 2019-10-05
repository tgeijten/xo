#include "path.h"
#include "xo/string/string_tools.h"

namespace xo
{
	path& path::replace_extension( const path& ext )
	{
		size_t n = data_.find_last_of( '.' );
		if ( n != string::npos ) data_.erase( n ); // remove existing extension

		if ( !ext.empty() )
			data_ += '.' + ext.data_;

		return *this;
	}

	path& path::remove_filename()
	{
		size_t n = last_separator_pos();
		if ( n != string::npos ) data_ = data_.substr( 0, n );
		else data_.clear();
		return *this;
	}

	path& path::replace_filename( const path& f )
	{
		remove_filename();
		return *this /= f;
	}

	path& path::make_preferred()
	{
		for ( char& c : data_ )
			if ( c == '/' || c == '\\' )
				c = preferred_separator;
		return *this;
	}

	path path::parent_path() const
	{
		size_t n = last_separator_pos();
		return n != string::npos ? path( data_.substr( 0, n ) ) : path();
	}

	path path::extension_no_dot() const
	{
		size_t n = data_.find_last_of( '.' );
		return n != string::npos ? path( data_.substr( n + 1 ) ) : path();
	}

	path path::filename() const
	{
		size_t n = last_separator_pos();
		return n != string::npos ? path( data_.substr( n + 1 ) ) : *this;
	}

	path path::stem() const
	{
		size_t n1 = last_separator_pos();
		size_t n2 = data_.find_last_of( '.' );
		if ( n1 == string::npos ) n1 = 0; else ++n1;
		return path( data_.substr( n1, ( n2 >= n1 ) ? n2 - n1 : string::npos ) );
	}

	bool path::empty() const
	{
		return data_.empty();
	}

	bool path::has_filename() const
	{
		return !empty()
			&& last_separator_pos() != data_.size() - 1
			&& !str_ends_with( data_, ":" );
	}

	bool path::has_parent_path() const
	{
		return last_separator_pos() != string::npos;
	}

	bool path::is_absolute() const
	{
		return str_begins_with( data_, '/' ) || data_.find( ':' ) != string::npos;
	}

	bool path::is_relative() const
	{
		return !is_absolute();
	}

	size_t path::last_separator_pos() const
	{
		return data_.find_last_of( "/\\" );
	}

	path& path::operator/=( const string_type& p )
	{
		if ( has_filename() )
			data_ += preferred_separator + trim_left_str( p, "/\\" );
		else data_ += p;
		return *this;
	}
	path& path::operator/=( const path& p ) { return *this /= p.str(); }

	path& path::operator+=( const string_type& p )
	{
		data_ += p;
		return *this;
	}
	path& path::operator+=( const path& p ) { return *this += p.str(); }

	path operator/( const path& p1, const path& p2 ) { return path( p1 ) /= p2; }
	path operator+( const path& p1, const path& p2 ) { return path( p1 ) += p2; }
	bool operator==( const path& p1, const path& p2 ) { return p1.str() == p2.str(); }
	bool operator!=( const path& p1, const path& p2 ) { return p1.str() != p2.str(); }
}
