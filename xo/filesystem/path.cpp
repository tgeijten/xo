#include "path.h"

#include <filesystem>

namespace xo
{
	const size_t path::npos = string::npos;

	path::path( std::filesystem::path& p ) :
		data_( p.string() )
	{}

	char path::preferred_separator()
	{
		return '/';
	}

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

	xo::path& path::make_preferred()
	{
		for ( char& c : data_ )
			if ( c == '/' || c == '\\' ) c = preferred_separator();
		return *this;
	}

	path path::parent_path() const
	{
		size_t n = last_separator_pos();
		return n != string::npos ? path( data_.substr( 0, n ) ) : path();
	}

	path path::extension() const
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
		return last_separator_pos() != data_.size() - 1;
	}

	bool path::has_parent_path() const
	{
		return last_separator_pos() != npos;
	}

	size_t path::last_separator_pos() const
	{
		return data_.find_last_of( "/\\" );
	}

	xo::path& path::operator/=( const xo::string& p )
	{
		data_ += has_filename() ? preferred_separator() + p : p;
		return *this;
	}

	xo::path& path::operator/=( const path& p )
	{
		return *this /= p.to_string();
	}

	path operator/( const path& p1, const string& p2 )
	{
		return p1.has_filename() ? path( p1.to_string() + path::preferred_separator() + p2 ) : path( p1.to_string() + p2 );
	}

	xo::path operator/( const path& p1, const path& p2 )
	{
		return p1 / p2.to_string();
	}

	path operator/( const path& p1, const char* p2 )
	{
		return p1 / string( p2 );
	}

	xo::path operator+( const path& p1, const string& p2 )
	{
		return path( p1.to_string() + p2 );
	}

	path operator+( const string& p1, const path& p2 )
	{
		return path( p1 + p2.to_string() );
	}

	bool operator!=( const path& p1, const path& p2 )
	{
		return p1.to_string() != p2.to_string();
	}

	bool operator==( const path& p1, const path& p2 )
	{
		return p1.to_string() == p2.to_string();
	}

}
