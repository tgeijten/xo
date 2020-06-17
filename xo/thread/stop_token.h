#pragma once

#include <atomic>
#include <memory>

namespace xo
{
	using stop_state = std::atomic_bool;
	using stop_state_ptr = std::shared_ptr<stop_state>;

	/// simplified version of c++20 stop_token
	struct stop_token
	{
		stop_token() = default;

		bool stop_requested() const { return stop_state_ && stop_state_->load(); }
		bool stop_possible() const { return stop_state_ != nullptr; }

	private:
		friend struct stop_source;
		stop_token( stop_state_ptr stop_state ) : stop_state_( std::move( stop_state ) ) {}
		stop_state_ptr stop_state_;
	};

	/// simplified version of c++20 stop_source
	struct stop_source
	{
		stop_source() : stop_state_( std::make_shared<stop_state>( false ) ) {}

		bool request_stop() { stop_state_->store( true ); return true; }

		stop_token get_token() const { return stop_token( stop_state_ ); }
		bool stop_requested() const { return stop_state_ && stop_state_->load(); }
		bool stop_possible() const { return stop_state_ != nullptr; }

	private:
		stop_state_ptr stop_state_;
	};
}
