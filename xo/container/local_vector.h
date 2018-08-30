#pragma once
#include <type_traits>

namespace xo
{
	template< typename T, size_t N >
	class local_vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		local_vector() : end_( data_ ) {}
		local_vector( size_t n, const T& v ) : end_( data_ + n ) { assign( v ); }
		~local_vector() { for ( auto it = begin(); it != end(); ++it ) { it->~T(); } }

		T& operator[]( index_t i ) { return *reinterpret_cast<T*>( data_ + i ); }
		const T& operator[]( index_t i ) const { return *reinterpret_cast<const T*>( data_ + i ); }

		T& at( size_t i ) { xo_error_if( i >= size(), "stackvec index out of bounds" ); return ( *this )[ i ]; }
		const T& at( size_t i ) const { xo_error_if( i >= size(), "stackvec index out of bounds" ); ( *this )data_[ i ]; }

		template<typename ...Args> void emplace_back( Args&&... args ) {
			xo_error_if( size() >= N, "stackvec buffer overflow" );
			new( end_ ) T( std::forward<Args>( args )... );	++end_;
		}

		void push_back( const T& e ) {
			xo_error_if( size() >= N, "stackvec buffer overflow" ); *end_++ = e;
			new( end_ ) e; ++end_;
		}

		void push_back( T&& e ) {
			xo_error_if( size() >= N, "stackvec buffer overflow" ); *end_++ = std::move( e );
		}

		iterator begin() { return data(); }
		const_iterator begin() const { return data(); }
		const_iterator cbegin() const { return data(); }

		iterator end() { return end_; }
		const_iterator end() const { return end_; }
		const_iterator cend() const { return end_; }

		size_t size() const { return end() - begin(); }

		void clear() { }

		T* data() { return reinterpret_cast<T*>( data_ ); }
		const T* data() const { return reinterpret_cast<const T*>( data_ ); }

	private:
		typename std::aligned_storage< sizeof( T ), alignof( T ) >::type data[ N ];
		T* end_;
	};
}
