#pragma once

#include <string>
#include <vector>

namespace xo
{
	class stack_string
	{
	public:
		stack_string() {}

		const std::string& str() const { return value_; }

		void set( std::string s ) { value_ = s; sizes_.assign( s.empty() ? 0 : 1, 0 ); }
		void push_back( const std::string& s ) { sizes_.push_back( value_.size() ); value_ += s; }
		void pop_back() { value_.resize( sizes_.back() ); sizes_.pop_back(); }

	private:
		std::string value_;
		std::vector< size_t > sizes_;
	};
}
