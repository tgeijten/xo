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
			pn[ "_label_" ] = label;
			pn[ "_info_" ] = info;
			pn[ "_type_" ] = get_type_class< T >();
		}

		template< typename T > T get( const string& id ) const { return data_.get_delimited< T >( id ); }
		template< typename T > void set( const string& id, const T& value ) { data_.set_delimited< T >( id, value ); }

		void load( const path& filename );
		void save( const path& filename ) const;

		bool empty() const { return data_.empty(); }

		const prop_node& data() const { return data_; }
		prop_node& data() { return data_; }

	private:
		prop_node data_;
	};
}
