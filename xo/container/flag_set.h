#pragma once

#include "xo/xo_types.h"
#include <initializer_list>

namespace xo
{
	/// class for defining flags in a bitset
	// #todo: use smart storage type
	template< typename EnumT, typename StorageT = uint64 >
	struct flag_set
	{
		flag_set() : data_( StorageT( 0 ) ) {}
		flag_set( const flag_set& o ) = default;
		flag_set( std::initializer_list< EnumT > flags ) : data_( StorageT( 0 ) ) { set( flags ); }
		flag_set( EnumT flag ) : data_( storage_bit( flag ) ) {}
		explicit flag_set( StorageT data ) : data_( data ) {}

		flag_set& operator=( const flag_set& o ) { data_ = o.data_; return *this; }

		inline bool get( EnumT index ) const { return ( data_ & storage_bit( index ) ) != 0; }
		template< EnumT index >	bool get() const { return ( data_ & storage_bit( index ) ) != 0; }
		inline bool operator()( EnumT index ) const { return get( index ); }

		flag_set& set( EnumT index, bool value ) { data_ = ( data_ & ~storage_bit( index ) ) | storage_bit( index, value ); return *this; }
		template< EnumT index > flag_set& set( bool value ) { data_ = ( data_ & ~storage_bit( index ) ) | storage_bit( index, value ); return *this; }

		flag_set& set( EnumT index ) { data_ |= storage_bit( index ); return *this; }
		template< EnumT index > flag_set& set() { data_ |= storage_bit( index ); return *this; }
		template< EnumT index > flag_set& clear() { data_ &= ~storage_bit( index ); return *this; }

		flag_set& toggle( EnumT index ) { set( index, !get( index ) ); return *this; }
		template< EnumT index > flag_set& toggle() { set<index>( !get<index>() ); return *this; }

		flag_set& set( std::initializer_list< EnumT > flags ) { for ( auto f : flags ) set( f ); return *this; }
		flag_set& set( std::initializer_list< EnumT > flags, bool value ) { for ( auto f : flags ) set( f, value ); return *this; }
		flag_set& clear( std::initializer_list< EnumT > flags ) { for ( auto f : flags ) clear( f ); return *this; }

		void clear() { data_ = StorageT(0); }
		bool any() const { return data_ != 0; }
		StorageT data() const { return data_; }

	private:
		static StorageT storage_bit( EnumT i ) { return StorageT( 1 ) << int( i ); }
		static StorageT storage_bit( EnumT i, bool value ) { return StorageT( value ) << int( i ); }
		StorageT data_;
	};
}
