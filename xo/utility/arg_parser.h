#pragma once

#include "xo/container/vector_type.h"
#include "xo/string/string_type.h"
#include "xo/string/string_tools.h"

namespace xo
{
	/// super light-weight arg parser
	class arg_parser
	{
	public:
		arg_parser( const int argc, const char** argv ) {
			for ( int i = 1; i < argc; ++i )
				args_.emplace_back( argv[i] );
		}

		index_t find_flag( const string& f ) const {
			for ( index_t idx = 0; idx < args_.size(); ++idx ) {
				auto dashes = count_dashes( idx );
				if ( dashes > 0 && arg( idx ).compare( dashes, string::npos, f ) == 0 )
					return idx;
			}
			return no_index;
		}

		bool has_flag( const string& f ) const {
			return find_flag( f ) != no_index;
		}

		template< typename T >
		T get( const string& f, T default_value ) const {
			if ( auto idx = find_flag( f ); idx != no_index && idx < size() - 1 && !str_begins_with( arg( idx + 1 ), '-' ) )
				from_str( arg( idx + 1 ), default_value );
			return default_value;
		}

		template< typename T >
		T get( index_t idx, T default_value ) const {
			index_t cur_idx = 0;
			for ( index_t i = 0; i < size() && cur_idx <= idx; ++i )
				if ( !is_flag( i ) )
					if ( cur_idx++ == idx )
						from_str( args_[i], default_value );
			return default_value;
		}

		const string& arg( index_t idx ) const {
			xo_assert( idx < args_.size() );
			return args_[idx];
		}

		size_t size() const { return args_.size(); }

	private:
		vector<string> args_;

		bool is_flag( index_t idx ) const {
			return count_dashes( idx ) > 0;
		}
		size_t count_dashes( index_t idx ) const {
			const auto& a = args_[idx];
			size_t c = 0;
			while ( c < a.size() && a[c] == '-' )
				++c;
			return c;
		}
	};
}
