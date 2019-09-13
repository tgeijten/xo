#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"

namespace xo
{
	class XO_API path
	{
	public:
		path() {}
		path( const path& p ) : data_( p.data_ ) {}
		path( path&& p ) noexcept : data_( std::move( p.data_ ) ) {}
		path( const char* p ) : data_( p ) {}
		explicit path( const string& p ) : data_( p ) {}
		explicit path( string&& p ) : data_( std::move( p ) ) {}

		path& operator=( const path& p ) { data_ = p.data_; return *this; }
		path& operator=( path&& p ) noexcept { data_ = std::move( p.data_ ); return *this; }
		path& operator=( const string& p ) { data_ = p; return *this; }
		path& operator=( string&& p ) { data_ = std::move( p ); return *this; }

		static char preferred_separator();

		const std::string& str() const { return data_; }
		const char* c_str() const { return data_.c_str(); }

		path& replace_extension( const path& ext = path() );
		path& remove_filename();
		path& replace_filename( const path& f = path() );
		path& make_preferred();
		path parent_path() const;
		path extension_no_dot() const;
		path filename() const;
		path stem() const;
		bool empty() const;
		path& operator/=( const path& p );
		path& operator/=( const xo::string& p );

		bool has_filename() const;
		bool has_parent_path() const;

		static const size_t npos;

	private:
		size_t last_separator_pos() const;
		std::string data_;
	};

	XO_API path operator/( const path& p1, const path& p2 );
	XO_API path operator/( const path& p1, const string& p2 );
	XO_API path operator/( const path& p1, const char* p2 );
	XO_API path operator+( const path& p1, const string& p2 );
	XO_API path operator+( const string& p1, const path& p2 );
	XO_API bool operator==( const path& p1, const path& p2 );
	XO_API bool operator!=( const path& p1, const path& p2 );
	inline bool operator<( const path& p1, const path& p2 ) { return p1.str() < p2.str(); }
}
