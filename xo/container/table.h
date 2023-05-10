#pragma once

#include "xo/xo_types.h"
#include "xo/container/container_tools.h"
#include "xo/container/label_vector.h"
#include "xo/string/string_type.h"
#include "xo/string/string_tools.h"
#include "xo/filesystem/path.h"

#include <vector>
#include <iostream>
#include <string>

namespace xo
{
	template< typename T, typename L = string >
	class table
	{
	public:
		table( size_t rows = 0, size_t cols = 0 ) : row_labels( rows ), col_labels( cols ), data( rows* cols ) {}
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

		index_t find_column( const L& label ) const { return col_labels.find( label ); }
		index_t find_row( const L& label ) const { return row_labels.find( label ); }

		index_t get_or_add_row( const L& label, const T& default_value = T() ) {
			auto idx = find_row( label );
			return idx == no_index ? add_row( label, default_value ) : idx;
		}

		index_t get_or_add_column( const L& label, const T& default_value = T() ) {
			auto idx = find_column( label );
			return idx == no_index ? add_column( label, default_value ) : idx;
		}

		void resize( size_t rows, size_t cols, const T& default_value = T() ) {
			xo_error_if( rows < row_size() || cols < column_size(), "tables cannot be shrinked" );

			// reorganize existing data
			data.resize( row_size() * cols );
			for ( index_t ri = rows; ri-- > 0; )
				for ( index_t ci = cols; ci-- > 0; )
					data[cols * ri + ci] = ( ri < row_size() && ci < column_size() ) ? data[column_size() * ri + ci] : default_value;
			col_labels.resize( cols );
			row_labels.resize( rows );
		}

		size_t row_size() const { return row_labels.size(); }
		size_t column_size() const { return col_labels.size(); }
		bool empty() const { return data.empty(); }

		const T& operator()( index_t row, index_t col ) const { xo_assert( row < row_size() && col < column_size() ); return data[row * column_size() + col]; }
		T& operator()( index_t row, index_t col ) { xo_assert( row < row_size() && col < column_size() ); return data[row * column_size() + col]; }

		const T& operator()( const L& row, const L& col ) const { operator()( find_row( row ), find_column( col ) ); }
		T& operator()( const L& row, const L& col ) { return operator()( get_or_add_row( row ), get_or_add_column( col ) ); }

		const T& operator()( index_t row, const L& col ) const { return operator()( row, find_column( col ) ); }

		const T& back_row( index_t col ) const { return operator()( row_size() - 1, col ); }
		T& back_row( index_t col ) { return operator()( row_size() - 1, col ); }

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
			str << t.row_labels[row];
			for ( index_t col = 0; col < t.column_size(); ++col )
				str << "\t" << t( row, col );
			str << std::endl;
		}

		return str;
	}

	template< typename T, typename S = string >
	table<T, S> read_table( std::istream& str, bool first_row_has_header = true ) {
		table<T, S> t;
		string header;
		std::getline( str, header );
		auto columns = split_str( header, " \t" );
		for ( index_t i = first_row_has_header ? 1 : 0; i < columns.size(); ++i )
			t.add_column( columns[i] );
		while ( str.good() ) {
			string row_label;
			str >> row_label;
			if ( str.good() ) {
				t.add_row( row_label );
				for ( index_t i = 0; i < t.column_size(); ++i )
					str >> t.back_row( i );
			}
		}
		return t;
	}

	template< typename T, typename S = string >
	table<T, S> load_table( const path& filename, bool first_row_has_header = true ) {
		auto str = std::ifstream( filename.str() );
		return read_table<T, S>( str, first_row_has_header );
	}
}
