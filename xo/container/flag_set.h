#pragma once

#include <initializer_list>

namespace xo
{
	/// class for defining flags in a bitset
	// TODO: use smart storage type
	template< typename EnumT, typename StorageT = unsigned int >
	struct flag_set
	{
		flag_set() : data( StorageT( 0 ) ) {}
		flag_set( const flag_set& o ) : data( o.data ) {}
		flag_set( std::initializer_list< EnumT > flags ) : data( StorageT( 0 ) ) { set( flags ); }

		flag_set& operator=( const flag_set& o ) { data = o.data; return *this; }

		inline const bool get( EnumT index ) const { return ( data & storage_bit( index ) ) != 0; }
		inline const bool operator()( EnumT index ) const { return get( index ); }
		template< EnumT index >	bool get() const { return ( data & storage_bit( index ) ) != 0; }

		flag_set& set( EnumT index, bool value ) { data = ( data & ~storage_bit( index ) ) | storage_bit( index, value ); return *this; }
		template< EnumT index > flag_set& set( bool value ) { data = ( data & ~storage_bit( index ) ) | storage_bit( index, value ); return *this; }

		flag_set& set( EnumT index ) { data |= storage_bit( index ); return *this; }
		template< EnumT index > flag_set& set() { data |= storage_bit( index ); return *this; }
		template< EnumT index > flag_set& clear() { data &= ~storage_bit( index ); return *this; }

		flag_set& set( std::initializer_list< EnumT > flags ) { for ( auto f : flags ) set( f ); return *this; }
		flag_set& clear( std::initializer_list< EnumT > flags ) { for ( auto f : flags ) clear( f ); return *this; }

		void clear() { data = StorageT(0); }
		bool any() const { return data != 0; }

	private:
		StorageT storage_bit( EnumT i ) const { return StorageT( 1 ) << int( i ); }
		StorageT storage_bit( EnumT i, bool value ) const { return StorageT( value ) << int( i ); }
		StorageT data;
	};
}
