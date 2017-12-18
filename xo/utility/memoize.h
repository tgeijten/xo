#pragma once

#include <functional>

namespace xo
{
	template <typename T>
	struct memoize;

	template< typename R, typename... Args >
	struct memoize< R( Args... ) >
	{
		memoize( std::function< R( Args... )> f  ) : func_( f ) {}

		R operator()( Args... args ) {
			auto tup = std::make_tuple( args... );
			auto it = mem_.find( tup );
			if ( it == mem_.end() ) {
				auto r = func_( args... );
				mem_[ tup ] = r;
				return r;
			}
			else
				return it->second;
		}
		
	private:
		std::map< std::tuple< Args... >, R > mem_;
		std::function< R( Args... ) > func_;
	};
}
