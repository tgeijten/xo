#pragma once

#include "xo/system/assert.h"
#include "xo/utility/pointer_iterator.h"
#include "xo/container/dynarray.h"
#include "xo/numerical/constants.h"
#include <cmath>

namespace xo
{
	template< typename T > struct dynvec
	{
		using value_type = T;
		using iterator = pointer_iterator< T >;
		using const_iterator = pointer_iterator< const T >;

		/// construction
		dynvec() : data_() {}
		dynvec( size_t n ) : data_( n ) {}
		dynvec( size_t n, const T& v ) : data_( n, v ) {}
		dynvec( const dynvec<T>& o ) : data_( o.data_ ) {}
		dynvec( dynvec<T>&& o ) : data_( std::move( o.data_ ) ) {}

		/// assignment
		dynvec<T>& operator=( const dynvec<T>& o ) { data_ = o.data_; return *this; }
		dynvec<T>& operator=( dynvec<T>&& o ) { data_ = std::move( o.data_ ); return *this; }

		/// element access
		const T& operator[]( index_t idx ) const { return data_[idx]; }
		T& operator[]( index_t idx ) { return data_[idx]; }
		iterator begin() { return data_.begin(); }
		iterator end() { return data_.end(); }
		const_iterator begin() const { return data_.begin(); }
		const_iterator end() const { return data_.end(); }

		// resize
		void resize( size_t n, const T& v = T() ) { data_.resize( n, v ); }

		/// properties
		size_t size() const { return end() - begin(); }
		bool empty() const { return begin() == end(); }
		T length() const { return std::sqrt( squared_length() ); }
		T squared_length() const { T sum = T(); for ( auto& e : data_ ) sum += e * e; return sum; }
		bool is_null() const { for ( auto& e : data_ ) if ( e != T( 0 ) ) return false; return true; }

	private:
		dynarray< T > data_;
	};

	/// template instantiations
	using dynvecf = dynvec< float >;
	using dynvecd = dynvec< double >;

	template< typename T > bool operator==( const dynvec<T>& v1, const dynvec<T>& v2 ) {
		xo_assert( v1.size() == v2.size() );
		for ( index_t i = 0; i < v1.size(); ++i )
			if ( v1[i] != v2[i] )
				return false;
		return true;
	}

	template< typename T > bool operator!=( const dynvec<T>& v1, const dynvec<T>& v2 ) { return !( v1 == v2 ); }

	/// Addition
	template< typename T > dynvec<T> operator+( const dynvec<T>& v1, const dynvec<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		dynvec<T> r( v1.size() );
		for ( index_t i = 0; i < r.size(); ++i )
			r[i] = v1[i] + v2[i];
		return r;
	}
	/// Addition
	template< typename T > dynvec<T>& operator+=( dynvec<T>& v1, const dynvec<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		for ( index_t i = 0; i < v1.size(); ++i )
			v1[i] += v2[i];
		return v1;
	}

	/// Subtraction
	template< typename T > dynvec<T> operator-( const dynvec<T>& v1, const dynvec<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		dynvec<T> r( v1.size() );
		for ( index_t i = 0; i < r.size(); ++i )
			r[i] = v1[i] - v2[i];
		return r;
	}
	/// Subtraction
	template< typename T > dynvec<T>& operator-=( dynvec<T>& v1, const dynvec<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		for ( index_t i = 0; i < v1.size(); ++i )
			v1[i] -= v2[i];
		return v1;
	}

	/// Negation
	template< typename T > dynvec<T> operator-( dynvec<T> v ) { for ( auto& e : v )	e = -e;	return v; }

	/// Scalar multiplication
	template< typename T, typename TS > dynvec<T> operator*( TS s, dynvec<T> v ) { for ( auto& e : v ) e *= s; return v; }
	template< typename T, typename TS > dynvec<T> operator*( dynvec<T> v, TS s ) { for ( auto& e : v ) e *= s; return v; }
	template< typename T, typename TS > dynvec<T>& operator*=( dynvec<T>& v, TS s ) { for ( auto& e : v ) e *= s; return v; }

	/// Scalar division
	template< typename T > dynvec<T> operator/( dynvec<T> v, T s ) { return v * inv( s ); }
	template< typename T > dynvec<T>& operator/=( dynvec<T>& v, T s ) { return v *= inv( s ); }

	/// Test if a vec3 is of unit length
	template< typename T > T is_normalized( const dynvec<T>& v ) { return equal( v.length(), T( 1 ) ); }

	/// Get distance between two dynvecs
	template< typename T > T distance( const dynvec<T>& v1, const dynvec<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		T sum = T( 0 );
		for ( index_t i = 0; i < v1.size(); ++i )
			sum += squared( v1[i] - v2[i] );
		return std::sqrt( sum );
	}

	/// Normalize
	template< typename T > T normalize( dynvec<T>& v ) { T l = v.length(); if ( l > num<T>::epsilon ) { v /= l; } return l; }
	template< typename T > dynvec<T> normalized( dynvec<T> v ) { normalize( v ); return v; }

	/// Dot product
	template< typename T > T dot_product( const dynvec<T>& v1, const dynvec<T>& v2 )
	{
		xo_assert( v1.size() == v2.size() );
		T r = T( 0 );
		for ( index_t i = 0; i < v1.size(); ++i )
			r += v1[i] * v2[i];
		return r;
	}

	/// stream out
	template< typename T > std::ostream& operator<<( std::ostream& str, const dynvec<T>& v ) {
		const auto separator = std::string( " " );
		if ( v.size() > 0 ) {
			str << v[0];
			for ( index_t i = 1; i < v.size(); ++i )
				str << separator << v[i];
		}
		return str;
	}

	/// stream in
	template< typename T > std::istream& operator>>( std::istream& str, dynvec<T>& v )
	{ for ( auto& e : v ) str >> e; return str; }
}
