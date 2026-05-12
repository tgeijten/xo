#pragma once

#include <string>
#include <vector>

namespace xo
{
	class stack_string
	{
	public:
		const std::string& str() const { return value_; }

		void set( const std::string& s ) { value_ = s; sizes_.assign( s.empty() ? 0 : 1, 0 ); }
		void push_back( const std::string& s ) { sizes_.push_back( value_.size() ); value_ += s; }
		void pop_back() { value_.resize( sizes_.back() ); sizes_.pop_back(); }
		bool empty() const { return value_.empty(); }
		size_t stack_size() const { return sizes_.size(); }
		size_t pos( size_t idx ) const { return idx < stack_size() ? sizes_[idx] : value_.size(); }

	private:
		std::string value_;
		std::vector<size_t> sizes_;
	};
}
