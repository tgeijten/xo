#pragma once

#include <memory>

namespace xo
{
	template< typename T > using u_ptr = std::unique_ptr< T >;
	template< typename T > using s_ptr = std::shared_ptr< T >;

	// Try to perform a dynamic_cast on a unique pointer
	// SUCCESS: ownership is transfered to the returned pointer
	// FAILURE: original pointer maintains ownership, returns nullptr
	template <typename TargetT, typename SourceT>
	u_ptr<TargetT> try_dynamic_unique_cast( u_ptr<SourceT>& ptr ) {
		if ( TargetT* tptr = dynamic_cast<TargetT*>( ptr.get() ) ) {
			ptr.release();
			return u_ptr<TargetT>( tptr );
		}
		else return u_ptr<TargetT>();
	}

	// Perform a dynamic_cast on a unique pointer
	// SUCCESS: ownership is transfered to the returned pointer
	// FAILURE: original pointer is deleted, returns nullptr
	template <typename TargetT, typename SourceT>
	u_ptr<TargetT> dynamic_unique_cast( u_ptr<SourceT>&& ptr ) {
		if ( TargetT* tptr = dynamic_cast<TargetT*>( ptr.get() ) ) {
			ptr.release();
			return u_ptr<TargetT>( tptr );
		}
		else return u_ptr<TargetT>();
	}
}
