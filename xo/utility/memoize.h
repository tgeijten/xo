#pragma once

#include <functional>
#include <mutex>
#include <shared_mutex>
#include "type_traits.h"

namespace xo
{
	template <typename T>
	struct memoize;

	template< typename R, typename... Args >
	struct memoize< R( Args... ) >
	{
		memoize( std::function< R( Args... )> f ) : func_( f ) {}

		const R& operator()( Args... args ) {
			auto tup = std::make_tuple( args... );
			auto it = mem_.find( tup );
			if ( it == mem_.end() ) {
				return mem_[tup] = func_( args... );
			}
			else
				return it->second;
		}

		size_t size() const { return mem_.size(); }

	private:
		using key_type = std::tuple<typename remove_reference<Args>::type...>;
		std::map< key_type, R > mem_;
		std::function< R( Args... ) > func_;
	};

	template <typename T>
	struct memoize_thread_safe;

	template< typename R, typename... Args >
	struct memoize_thread_safe< R( Args... ) >
	{
		memoize_thread_safe( std::function< R( Args... )> f ) : func_( f ) {}

		const R& operator()( Args... args ) {
			auto tup = std::make_tuple( args... );
			std::shared_lock read_lock( mutex_ );
			auto it = mem_.find( tup );
			if ( it == mem_.end() ) {
				read_lock.unlock();
				std::unique_lock write_lock( mutex_ );
				try {
					if ( it = mem_.find( tup ); it != mem_.end() )
						return it->second; // another thread got a unique_lock first
					else
						return mem_[tup] = func_( args... );
				}
				catch ( std::exception& e ) {
					xo_error( e.what() );
				}
			}
			else
				return it->second;
		}

		size_t size() { std::shared_lock read_lock( mutex_ ); return mem_.size(); }

	private:
		using key_type = std::tuple<typename remove_reference<Args>::type...>;
		std::map< key_type, R > mem_;
		std::function< R( Args... ) > func_;
		std::shared_mutex mutex_;
	};
}
