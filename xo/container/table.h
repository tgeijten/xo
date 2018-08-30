#pragma once

#include "xo/utility/types.h"
#include <iosfwd>
#include <vector>
#include "xo/container/container_tools.h"
#include "xo/container/label_vector.h"
#include "xo/string/string_type.h"

namespace xo
{
	template< typename T, typename L = string >
	class table
	{
	public:
		table( size_t rows = 0, size_t cols = 0 ) : row_labels( rows ), col_labels( cols ), data( rows * cols ) {}
		~table() {}

		index_t add_row( const L& label, const T& default_value = T() ) {
			row_labels.add( label );
			data.resize( row_size() * column_size(), default_value );
			return row_size() - 1;
		}

		index_t add_column( const L& label, const T& default_value = T() ) {
			resize( row_size(), column_size() + 1, default_value );
			return col_labels.set( column_size() - 1, label );
		}

		index_t get_or_add_row( const L& label, const T& default_value = T() ) {
			auto idx = row_labels.find( label );
			return idx == no_index ? add_row( label, default_value ) : idx;
		}

		index_t get_or_add_column( const L& label, const T& default_value = T() ) {
			auto idx = col_labels.find( label );
			return idx == no_index ? add_column( label, default_value ) : idx;
		}

		void resize( size_t rows, size_t cols, const T& default_value = T() ) {
			xo_error_if( rows < row_size() || cols < column_size(), "tables cannot be shrinked" );

			// reorganize existing data
			data.resize( row_size() * cols );
			for ( index_t ri = rows; ri-- > 0; )
				for ( index_t ci = cols; ci-- > 0; )
					data[ cols * ri + ci ] = ( ri < row_size() && ci < column_size() ) ? data[ column_size() * ri + ci ] : default_value;
			col_labels.resize( cols );
			row_labels.resize( rows );
		}

		size_t row_size() const { return row_labels.size(); }
		size_t column_size() const { return col_labels.size(); }

		const T& operator()( index_t row, index_t col ) const { xo_assert( row < row_size() && col < column_size() ); return data[ row * column_size() + col ]; }
		T& operator()( index_t row, index_t col ) { xo_assert( row < row_size() && col < column_size() ); return data[ row * column_size() + col ]; }

		const T& operator()( const L& row, const L& col ) const { operator()( row_index( row ), col_index( col ) ); }
		T& operator()( const L& row, const L& col ) { return operator()( get_or_add_row( row ), get_or_add_column( col ) ); }

		template< typename T1 >
		friend std::ostream& operator<<( std::ostream& str, const table< T1 >& t );

	private:

		label_vector< L > row_labels;
		label_vector< L > col_labels;
		std::vector< T > data;
	};

	template< typename T >
	std::ostream& operator<<( std::ostream& str, const table< T >& t ) {
		str << "Row";
		for ( auto& rl : t.col_labels )
			str << "\t" << rl;
		str << std::endl;

		for ( index_t row = 0; row < t.row_size(); ++row )
		{
			str << t.row_labels[ row ];
			for ( index_t col = 0; col < t.column_size(); ++col )
				str << "\t" << t( row, col );
			str << std::endl;
		}

		return str;
	}
}
