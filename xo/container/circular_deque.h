#pragma once

#include "xo/xo_types.h"
#include "xo/system/assert.h"
#include <vector>

namespace xo
{
	template< typename T >
	class circular_deque
	{
	public:
		using value_type = T;
		using buffer_type = std::vector< T >;

		circular_deque( size_t init_capacity = 0 ) : front_( 0 ), size_( 0 ), buffer_( init_capacity ) {}
		~circular_deque() {}

		void push_back( const T& value ) {
			if ( ++size_ > capacity() ) buffer_.insert( buffer_.begin() + ( front_ + size_ - 1 ), value );
			else back() = value;
		}
		void push_front( const T& value ) {
			if ( ++size_ > capacity() ) buffer_.insert( buffer_.begin() + front_, value );
			else { front_ = ( front_ + capacity() - 1 ) % capacity(); front() = value; }
		}

		T& operator[]( index_t idx ) { return buffer_[( front_ + idx ) % buffer_.size()]; }
		const T& operator[]( index_t idx ) const { return buffer_[( front_ + idx ) % buffer_.size()]; }

		void pop_back() { xo_assert( size_ > 0 ); --size_; }
		void pop_front() { xo_assert( size_ > 0 ); front_ = ( front_ + 1 ) % capacity(); --size_; }

		T& back() { xo_assert( size_ > 0 ); return buffer_[( front_ + size_ - 1 ) % capacity()]; }
		const T& back() const { xo_assert( size_ > 0 ); return buffer_[( front_ + size_ ) % capacity()]; }

		T& front() { xo_assert( size_ > 0 ); return buffer_[front_]; }
		const T& front() const { xo_assert( size_ > 0 ); return buffer_[front_]; }

		size_t size() const { return size_; }

		bool empty() const { return size_ == 0; }
		bool full() const { return size() == capacity(); }
		void clear() { front_ = size_ = 0; }

		void reserve( size_t s ) { buffer_.resize( s ); }
		size_t capacity() const { return buffer_.size(); }

		template< typename IT > struct iterator_impl
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = IT;
			using difference_type = size_t;
			using pointer = IT*;
			using reference = IT&;
			iterator_impl( size_t index, const buffer_type& buffer ) : index_( index ), buffer_( buffer ) {}
			iterator_impl< IT >& operator++() { ++index_; return *this; }
			iterator_impl< IT >& operator++( int ) { auto ret = *this; ++index_; return ret; }
			iterator_impl< IT > operator+( int v ) const { return iterator_impl< IT >( index_ + v, buffer_ ); }
			iterator_impl< IT > operator-( int v ) const { return iterator_impl< IT >( index_ - v, buffer_ ); }
			auto operator-( const iterator_impl< IT >& other ) const { return index_ - other.index_; }
			bool operator==( const iterator_impl< IT >& other ) { return other.index_ == index_; }
			bool operator!=( const iterator_impl< IT >& other ) { return other.index_ != index_; }
			IT& operator*() { return const_cast<IT&>( buffer_[index_ % buffer_.size()] ); }
			IT* operator->() { return const_cast<IT*>( &buffer_[index_ % buffer_.size()] ); }

			size_t index_;
			const buffer_type& buffer_;
		};
		using iterator = iterator_impl< T >;
		using const_iterator = iterator_impl< const T >;

		iterator begin() { return iterator( front_, buffer_ ); }
		iterator end() { return iterator( front_ + size_, buffer_ ); }
		const_iterator begin() const { return const_iterator( front_, buffer_ ); }
		const_iterator end() const { return const_iterator( front_ + size_, buffer_ ); }

	private:
		size_t front_;
		size_t size_;
		buffer_type buffer_;
	};
}
