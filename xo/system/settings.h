#pragma once

#include "xo/string/string_type.h"
#include "xo/container/prop_node.h"
#include "type_class.h"

namespace xo
{
	class XO_API settings
	{
	public:
		settings( prop_node schema, const path& filename = path() );

		template< typename T > T get( const string& id ) const { return data_.get_delimited< T >( id ); }

		template< typename T > T set( const string& id, const T& value ) {
			auto& schema_pn = schema_.get_child_delimited( id );
			auto& data_pn = data_.set_delimited( id, value );
			fix_setting( data_pn, schema_pn );
			return data_pn.get<T>();
		}

		void set( prop_node data );

		const prop_node& data() const { return data_; }
		const prop_node& schema() const { return schema_; }
		const prop_node& schema( const string& id ) { return schema_.get_child_delimited( id ); }

		void load( const path& filename = path() );
		void save( const path& filename = path() );
		const path& data_file() const { return data_file_; }

	private:
		void fix_setting( prop_node& setting, const prop_node& schema );
		void fix_settings( prop_node& settings, const prop_node& schema );

		prop_node data_;
		const prop_node schema_;
		path data_file_;
	};
}
