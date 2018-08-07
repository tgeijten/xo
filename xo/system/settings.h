#pragma once

#include "xo/string/string_type.h"
#include "xo/container/prop_node.h"
#include "type_class.h"

namespace xo
{
	class XO_API settings
	{
	public:
		settings() {}

		template< typename T >
		void add( const string& id, const string& label, const T& default_value, const string& info = "" ) {
			auto& pn = data_.set_delimited( id, default_value );
			set_meta_data( pn, label, info, get_type_class< T >() );
		}

		template< typename T > T get( const string& id ) const { return data_.get_delimited< T >( id ); }
		template< typename T > void set( const string& id, const T& value ) {
			auto pn = data_.try_get_child_delimited( id );
			xo_error_if( !pn, "Could not find setting " + id );
			pn->set_value( value );
		}

		void load( const path& filename );
		void save( const path& filename ) const;

		bool empty() const { return data_.empty(); }

		const prop_node& data() const { return data_; }

		void inject_settings( const prop_node& settings );
		prop_node extract_settings() const;

	private:
		void set_meta_data( prop_node& pn, const string& label, const string& info, type_class t );
		prop_node data_;
	};
}
