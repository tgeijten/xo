#pragma once

#include <string>
#include <vector>

namespace xo
{
	class stack_string
	{
	public:
		stack_string() {}
		stack_string& operator=( std::string s ) { value_ = s; if ( s.empty() ) sizes_.clear(); else sizes_.assign( 1, 0 ); }

		operator const std::string&() const { return value_; }
		void push_back( const std::string& s ) { sizes_.push_back( value_.size() ); value_ += s; }
		void pop_back() { value_.resize( sizes_.back() ); sizes_.pop_back(); }

	private:
		std::string value_;
		std::vector< size_t > sizes_;
	};
}
