#pragma once

#include "xo/system/xo_config.h"
#include "xo/string/string_type.h"

namespace xo
{
	class XO_API path
	{
	public:
		using string_type = ::xo::string;

		path() = default;
		path( const char* p ) : data_( p ) {}
		path( const string_type& p ) : data_( p ) {}
		path( string_type&& p ) : data_( std::move( p ) ) {}
		path& operator=( const char* p ) { data_ = p; return *this; }
		path& operator=( const string_type& p ) { data_ = p; return *this; }
		path& operator=( string_type&& p ) { data_ = std::move( p ); return *this; }

		static constexpr char preferred_separator = '/';

		const std::string& str() const { return data_; }
		const char* c_str() const { return data_.c_str(); }

		path& replace_extension( const path& ext = path() );
		path& remove_filename();
		path& replace_filename( const path& f = path() );
		path& replace_stem( const path& stem = path() );
		path& concat_stem( const string_type& postfix );
		path& make_preferred();
		path parent_path() const;
		path extension() const;
		path extension_no_dot() const;
		path filename() const;
		path stem() const;
		bool empty() const;

		path& operator/=( const path& p );
		path& operator/=( const string_type& p );
		path& operator+=( const path& p );
		path& operator+=( const string_type& p );

		bool has_filename() const;
		bool has_parent_path() const;

		bool is_absolute() const;
		bool is_relative() const;

	private:
		size_t last_separator_pos() const;
		string_type data_;
	};

	XO_API path operator/( const path& p1, const path& p2 );
	XO_API path operator+( const path& p1, const path& p2 );
	XO_API bool operator==( const path& p1, const path& p2 );
	XO_API bool operator!=( const path& p1, const path& p2 );
	inline bool operator<( const path& p1, const path& p2 ) { return p1.str() < p2.str(); }
}
